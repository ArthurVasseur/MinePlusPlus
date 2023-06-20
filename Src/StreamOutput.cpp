//
// Created by arthu on 24/09/2022.
//

#include <format>
#include "StreamOutput.hpp"

namespace MinePP
{
	std::ostream& operator<<(std::ostream& os, const Byte& byte)
	{
		os << std::format("{:02x}", (UByte)byte);
		return os;
	}

	std::ostream& operator<<(std::ostream& os, const UByte& byte)
	{
		os << std::format("{:02x}", byte);
		return os;
	}

	std::ostream& operator<<(std::ostream& os, const Buffer& buffer)
	{
		os << "<Buffer Size: " << buffer.size() << ", Data : ";
		for (auto& byte: buffer)
			os << byte << " ";
		os << ">";
		return os;
	}

	std::ostream& operator<<(std::ostream& os, const String& string)
	{
		os << "<String Size: " << string.size() << ", Data: " << string.c_str() << " >";
		return os;
	}

	std::ostream& operator<<(std::ostream& os, const std::istream& is)
	{
//		os << "<Stream Size: " << is << ", Data: ";
		auto buff = is.rdbuf();
		std::cout << buff << std::endl;
		return os;
	}
} // MinePP