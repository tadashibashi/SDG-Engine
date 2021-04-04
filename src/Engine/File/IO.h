//
// Contains low level functions for file IO
//
#pragma once
#include <Engine/Core.h>
#include <vector>
#include <string>

namespace SDG::IO
{
		// Reads a file and dumps its contents into the designated buffer.
		bool ReadFile(const std::string &filepath,
			std::vector<unsigned char> *buffer);

		// Loads a PNG file with RGBA format and gets its width/height.
		bool DecodePNG(const std::string &filepath,
			std::vector<unsigned char> *buffer,
			unsigned long *width = nullptr, unsigned long *height = nullptr);

		// Decrypts a PVRCCZ file with RGBA format and gets its width/height.
		// File must be a power of 2 and equal width/height.
		bool DecodePVRCCZ(const std::string &filepath,
			std::vector<unsigned char> *buffer,
			unsigned long *width = nullptr, unsigned long *height = nullptr);

		// Unlocks PVRCCZ file encryption.
		void SetKeyPart(int index, uint32_t key);
}
