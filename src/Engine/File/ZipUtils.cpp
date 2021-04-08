/*
 * Modified version of the cocos2d for iPhone ZipUtils.cpp
 */
#include "ZipUtils.h"
#include "IO.h"
#include "SwapEndian.h"
#include <Engine/Log.h>

#include <zlib/zlib.h>

#include <cstdlib>
#include <cassert>
#include <stdexcept>
#include <fstream>

 // memory in iPhone is precious
 // Should buffer factor be 1.5 instead of 2 ?
#define BUFFER_INC_FACTOR (2)

static int inflateMemoryWithHint(unsigned char *in, unsigned int inLength, unsigned char **out, unsigned int *outLength, unsigned int outLenghtHint)
{
    /* ret value */
    int err = Z_OK;

    int bufferSize = (int)outLenghtHint;
    *out = (unsigned char *)malloc(bufferSize);

    z_stream d_stream; /* decompression stream */
    d_stream.zalloc = (alloc_func)nullptr;
    d_stream.zfree = (free_func)nullptr;
    d_stream.opaque = (voidpf)nullptr;

    d_stream.next_in = in;
    d_stream.avail_in = inLength;
    d_stream.next_out = *out;
    d_stream.avail_out = bufferSize;

    /* window size to hold 256k */
    if ((err = inflateInit2(&d_stream, 15 + 32)) != Z_OK)
        return err;

    for (;;) {
        err = inflate(&d_stream, Z_NO_FLUSH);

        if (err == Z_STREAM_END)
            break;

        switch (err) {
        case Z_NEED_DICT:
            err = Z_DATA_ERROR;
        case Z_DATA_ERROR: case Z_MEM_ERROR:
            inflateEnd(&d_stream);
            return err;
        default:

            break;
        }

        // not enough memory ?
        if (err != Z_STREAM_END) {

            unsigned char *tmp = (unsigned char *)realloc(*out, bufferSize * BUFFER_INC_FACTOR);

            /* not enough memory, ouch */
            if (!tmp) {
                SDG_ERR("cocos2d: ZipUtils: realloc failed. Not enough memory.");
                inflateEnd(&d_stream);
                return Z_MEM_ERROR;
            }
            /* only assign to *out if tmp is valid. it's not guaranteed that realloc will reuse the memory */
            *out = tmp;

            d_stream.next_out = *out + bufferSize;
            d_stream.avail_out = bufferSize;
            bufferSize *= BUFFER_INC_FACTOR;
        }
    }


    *outLength = bufferSize - d_stream.avail_out;
    err = inflateEnd(&d_stream);
    return err;
}

int ccInflateMemoryWithHint(unsigned char *in, unsigned int inLength, unsigned char **out, unsigned int outLengthHint)
{
    unsigned int outLength = 0;
    int err = inflateMemoryWithHint(in, inLength, out, &outLength, outLengthHint);

    if (err != Z_OK || *out == nullptr) {
        if (err == Z_MEM_ERROR)
            SDG_ERR("ZipUtils: Out of memory while decompressing map data!");

        else if (err == Z_VERSION_ERROR)
            SDG_ERR("ZipUtils: Incompatible zlib version!");

        else if (err == Z_DATA_ERROR)
            SDG_ERR("ZipUtils: Incorrect zlib compressed data!");

        else
            SDG_ERR("ZipUtils: Unknown error while decompressing map data!");

        free(*out);
        *out = nullptr;
        outLength = 0;
    }

    return (int)outLength;
}

int ccInflateMemory(unsigned char *in, unsigned int inLength, unsigned char **out)
{
    // 256k for hint
    return ccInflateMemoryWithHint(in, inLength, out, 256 * 1024);
}

int ccInflateGZipFile(const char *path, unsigned char **out)
{
    int len;
    unsigned int offset = 0;
    if (!out || !(&*out)) {
        SDG_ERR("ccInflateGZipFile: invalid 'out' parameter");
        return -1;
    }

    gzFile inFile = gzopen(path, "rb");
    if (inFile == nullptr) {
        SDG_ERR(std::string("cocos2d: ZipUtils: error opening gzip file: ") + path);
        return -1;
    }

    /* 512k initial decompress buffer */
    int bufferSize = 512 * 1024;
    unsigned int totalBufferSize = bufferSize;

    *out = (unsigned char *)malloc(bufferSize);
    if (!out) {
        SDG_ERR("cocos2d: ZipUtils: out of memory");
        return -1;
    }

    for (;;) {
        len = gzread(inFile, *out + offset, bufferSize);
        if (len < 0) {
            free(*out);
            *out = nullptr;
            SDG_ERR("cocos2d: ZipUtils: error in gzread");
            return -1;
        }
        if (len == 0)
            break;

        offset += len;

        // finish reading the file
        if (len < bufferSize)
            break;

        bufferSize *= BUFFER_INC_FACTOR;
        totalBufferSize += bufferSize;
        unsigned char *tmp = (unsigned char *)realloc(*out, totalBufferSize);

        if (!tmp) {
            free(*out);
            *out = nullptr;
            SDG_ERR("cocos2d: ZipUtils: out of memory");
            return -1;
        }

        *out = tmp;
    }

    if (gzclose(inFile) != Z_OK)
        SDG_CORE_ERR("cocos2d: ZipUtils: gzclose failed");

    return (int)offset;
}

typedef struct {
    uint8_t			sig[4];				// signature. Should be 'CCZp' 4 bytes
    uint16_t		compression_type;	// should 0
    uint16_t		version;			// should be 2 (although version type==1 is also supported)
    uint32_t		checksum;			// Checksum
    uint32_t		len;				// size of the uncompressed file
} CCPHeader;

static uint32_t caw_key[4] = { 0,0,0,0 };
static uint32_t caw_longKey[1024];
static bool caw_longKeyValid = false;

void caw_setkey_part(int index, uint32_t value)
{
    assert(index >= 0);
    assert(index < 4);
    if (caw_key[index] != value)
    {
        caw_key[index] = value;
        caw_longKeyValid = false;
    }
}

static inline void caw_encdec(uint32_t *data, int len)
{
    const int enclen = 1024;
    const int securelen = 512;
    const int distance = 64;

    // check if key was set
    // make sure to call caw_setkey_part() for all 4 key parts
    assert(caw_key[0] != 0);
    assert(caw_key[1] != 0);
    assert(caw_key[2] != 0);
    assert(caw_key[3] != 0);

    // create long key
    if (!caw_longKeyValid)
    {
        uint32_t y;
        unsigned int p, rounds = 6, e;

        uint32_t sum = 0;
        uint32_t z = caw_longKey[enclen - 1];
        do
        {
#define DELTA 0x9e3779b9
#define MX (((z>>5^y<<2) + (y>>3^z<<4)) ^ ((sum^y) + (caw_key[(p&3)^e] ^ z)))

            sum += DELTA;
            e = (sum >> 2) & 3;
            for (p = 0; p < enclen - 1; p++)
            {
                y = caw_longKey[p + 1];
                z = caw_longKey[p] += MX;
            }
            y = caw_longKey[0];
            z = caw_longKey[enclen - 1] += MX;
        } while (--rounds);

        caw_longKeyValid = true;
    }

    int b = 0;
    int i = 0;

    // encrypt first part completely
    for (; i < len && i < securelen; i++)
    {
        data[i] ^= caw_longKey[b++];
        if (b >= enclen)
        {
            b = 0;
        }
    }

    // encrypt second section partially
    for (; i < len; i += distance)
    {
        data[i] ^= caw_longKey[b++];
        if (b >= enclen)
        {
            b = 0;
        }
    }
}

static inline uint32_t caw_checksum(const uint32_t *data, int len)
{
    uint32_t cs = 0;
    const int cslen = 128;
    len = (len < cslen) ? len : cslen;
    for (int i = 0; i < len; i++)
    {
        cs = cs ^ data[i];
    }
    return cs;
}

int ccInflateCCZFile(const char *path, std::vector<unsigned char> *out)
{
    printf("inflating: %s\n", path);

    if (!out || !(&*out))
    {
        SDG_CORE_ERR("ccInflateCCZFile: invalid 'out' parameter");
        return -1;
    }

    // load file into memory
    std::vector<unsigned char>compressed;
    SDG::IO::ReadFile(path, &compressed);

    // IO length check.
    uint32_t fileLen = (uint32_t)compressed.size();
    if (fileLen < 0)
    {
        SDG_CORE_ERR("Error loading CCZ compressed file");
        return -1;
    }

    uint32_t len, headerSize;

    if (compressed[0] == 'C' && compressed[1] == 'C' && compressed[2] == 'Z' && compressed[3] == '!')
    {
        // standard ccz file
        struct CCZHeader *header = (struct CCZHeader *)compressed.data();

        // verify header version
        uint16_t version = SDG::SwapEndian(header->version);

        if (version > 2) {
            SDG_CORE_ERR("cocos2d: Unsupported CCZ header format");
            return -1;
        }

        // verify compression format
        if (header->compression_type != CCZ_COMPRESSION_ZLIB) {
            SDG_CORE_ERR("cocos2d: CCZ Unsupported compression method");
            return -1;
        }

        len = SDG::SwapEndian(header->len);

        headerSize = sizeof(struct CCZHeader);
    }
    else if (compressed[0] == 'C' && compressed[1] == 'C' && compressed[2] == 'Z' && compressed[3] == 'p')
    {
        // encrypted ccz file
        CCPHeader *header = (CCPHeader *)compressed.data();

        // verify header version
        uint16_t version = SDG::SwapEndian(header->version);
        if (version > 0) {
            SDG_CORE_ERR("Unsupported CCZ header format");
            return -1;
        }

        // verify compression format
        if (SDG::SwapEndian(header->compression_type) != 0) {
            SDG_CORE_ERR("CCZ Unsupported compression method");
            return -1;
        }

        // decrypt
        headerSize = sizeof(CCPHeader);
        uint32_t *ints = (uint32_t *)(compressed.data() + 12);
        int enclen = ((int)fileLen - 12) / 4;

        caw_encdec(ints, enclen);

        len = SDG::SwapEndian(header->len);
#ifndef NDEBUG
        // verify checksum in debug mode
        uint32_t calculated = caw_checksum(ints, enclen);
        uint32_t required = SDG::SwapEndian(header->checksum);
        if (calculated != required)
        {
            SDG_CORE_ERR("Can't decrypt image file: Invalid decryption key");
            return -1;
        }
#endif
    }
    else
    {
        SDG_CORE_ERR("Invalid CCZ file");
        return -1;
    }

    unsigned char *buf = new unsigned char[len];

    uLongf destlen = len;
    uLongf source = (uLongf)(uintptr_t)compressed.data() + (uLongf)headerSize;
    int ret = uncompress(buf, &destlen, (Bytef *)(uintptr_t)source, fileLen - headerSize);

    if (ret != Z_OK)
    {
        SDG_CORE_ERR("Failed to uncompress data");
        delete[] buf;
        return -1;
    }
    std::vector<unsigned char> buf2;
    std::copy(buf, buf + destlen, std::back_inserter(buf2));

    delete[] buf;

    out->swap(buf2);

    return len;
}
