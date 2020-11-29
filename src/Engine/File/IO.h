#pragma once
#include <Engine/Core.h>
#include <vector>
#include <string>

namespace SDG::IO
{
		// Reads a file and dumps its contents into the designated buffer.
		bool SDG_API ReadFile(const std::string &filepath,
			std::vector<unsigned char> *buffer);

		// Loads a PNG file with RGBA format and gets its width and height.
		bool SDG_API DecodePNG(const std::string &filepath,
			std::vector<unsigned char> *buffer,
			unsigned long *width = nullptr, unsigned long *height = nullptr);

		// Decrypts a PVRCCZ file with RGBA format and gets its width/height.
		bool SDG_API DecodePVRCCZ(const std::string &filepath,
			std::vector<unsigned char> *buffer,
			unsigned long *width = nullptr, unsigned long *height = nullptr);

		// Unlocks PVRCCZ file encryption.
		void SDG_API SetKeyPart(int index, uint32_t key);
}
