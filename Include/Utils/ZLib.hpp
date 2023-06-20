//
// Created by arthur on 14/10/2022.
//

#ifndef MINEPLUSPLUS_ZLIB_HPP
#define MINEPLUSPLUS_ZLIB_HPP

#include <string_view>
#include <vector>
#include "Types.hpp"

namespace MinePP
{
	class Zlib
	{
	public:
		static std::vector<MinePP::Byte> Uncompress(std::string_view file);
		static void Compress(std::string_view file, std::vector<MinePP::Byte>);
	};
}
#endif //MINEPLUSPLUS_ZLIB_HPP
