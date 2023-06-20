//
// Created by arthur on 14/10/2022.
//
#include <iostream>
#include <array>
#include "zlib.h"
#include "Utils/ZLib.hpp"

namespace MinePP
{
	std::vector<MinePP::Byte> Zlib::Uncompress(std::string_view file)
	{
		gzFile nbt = gzopen(file.data(), "rb");
		if (nbt == nullptr)
		{
			std::cerr << "Failed to open file" << std::endl;
			return {};
		}

		std::vector<MinePP::Byte> buffer;
		std::array<MinePP::Byte, 1024> tmp {};
		while (true)
		{
			auto readSize = gzread(nbt, tmp.data(), tmp.size());
			if (readSize == 0)
				break;
			buffer.insert(buffer.end(), tmp.begin(), tmp.begin() + readSize);
		}
		gzclose(nbt);
		return buffer;
	}

	void Zlib::Compress(std::string_view file, Buffer buffer)
	{
		gzFile nbt = gzopen(file.data(), "wb");
		if (nbt == nullptr)
		{
			std::cerr << "Failed to open file" << std::endl;
			return;
		}
		gzwrite(nbt, buffer.data(), buffer.size());
		gzclose(nbt);
	}
}
