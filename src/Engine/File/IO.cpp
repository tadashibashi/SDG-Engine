#include "IO.h"
#include "PicoPNG.h"
#include "ZipUtils.h"
#include "SwapEndian.h"
#include <Engine/Log.h>

#include <fstream>

bool SDG::IO::ReadFile(const std::string &filepath, std::vector<unsigned char> *buffer)
{
    std::ifstream file(filepath, std::ios::binary);
    if (!file.is_open()) {
        perror(filepath.c_str());
        return false;
    }

    // seek to the end
    file.seekg(0, std::ios::end);

    // get the file size
    std::streamsize file_size = file.tellg();

    file.seekg(0, std::ios::beg); // move back to start

    file_size -= file.tellg();    // disregard file header metadata

    unsigned char *temp = new unsigned char[file_size];

    if (!file.read((char *)temp, file_size)) {
        file.close();
        std::perror(filepath.c_str());
        throw std::runtime_error("There was a failure to read file \"" + filepath + "\"");
    }

    std::vector<unsigned char> temp2;
    // commit changes
    std::copy(temp, temp + file_size, std::back_inserter(temp2));
    delete[] temp;

    buffer->swap(temp2);

    // clean up
    file.close();

    return true;
}

void SDG::IO::SetKeyPart(int index, uint32_t key)
{
    caw_setkey_part(index, key);
}

bool SDG::IO::DecodePNG(const std::string &filepath,
    std::vector<unsigned char> *buffer,
    unsigned long *width, unsigned long *height)
{
    std::vector<unsigned char> in;
    if (!ReadFile(filepath, &in)) {
        SDG_CORE_ERR("IO::DecodePNG failed. Problem while reading "
            "file at \"" + filepath + "\"");
        return false;
    }
    unsigned long temp_w, temp_h;

    if (int result = (decodePNG(*buffer, temp_w, temp_h, in.data(), in.size()) != 0)) {
        SDG_CORE_ERR("PicoPNG: Problem during decodePNG with error code ", result);
        return false;
    }

    // Commit 
    if (width)
        *width = temp_w;
    if (height)
        *height = temp_h;
    return true;
}

// For detailed information on each member, check out the PVR file format specification
struct PVRHeader {
    uint32_t version;
    uint32_t flags;
    uint64_t pixel_format;
    uint32_t color_space;
    uint32_t channel_type;
    uint32_t height;
    uint32_t width;
    uint32_t depth;
    uint32_t num_surfaces;
    uint32_t num_faces;
    uint32_t mipmap_count;
    uint32_t metadata_size;

    // Set only once after construction.
    void set_endian()
    {
        if (version == 0x50565203) // endianness does not match
        {
            version = SDG::SwapEndian(version);
            flags = SDG::SwapEndian(flags);
            pixel_format = SDG::SwapEndian(pixel_format);
            color_space = SDG::SwapEndian(color_space);
            channel_type = SDG::SwapEndian(channel_type);
            height = SDG::SwapEndian(height);
            width = SDG::SwapEndian(width);
            depth = SDG::SwapEndian(depth);
            num_surfaces = SDG::SwapEndian(num_surfaces);
            num_faces = SDG::SwapEndian(num_faces);
            mipmap_count = SDG::SwapEndian(mipmap_count);
            metadata_size = SDG::SwapEndian(metadata_size);
        }
    }

    size_t get_size() const
    {
        return sizeof(PVRHeader) + metadata_size;
    }
};

bool SDG::IO::DecodePVRCCZ(const std::string &filepath, std::vector<unsigned char> *buffer,
    unsigned long *width, unsigned long *height)
{
    // Inflate CCZ compression
    std::vector<unsigned char> temp;
    int file_size = ccInflateCCZFile(filepath.c_str(), &temp);

    // Get PVR header info
    PVRHeader pvr = *(PVRHeader *)temp.data();
    pvr.set_endian();

    // Read PVR header info
    unsigned long temp_w = pvr.width;
    unsigned long temp_h = pvr.height;

    // Strip PVR header info
    temp.erase(temp.begin(), temp.begin() + pvr.get_size());

    // Commit data
    buffer->swap(temp);

    if (width)
        *width = temp_w;
    if (height)
        *height = temp_h;

    return true;
}
