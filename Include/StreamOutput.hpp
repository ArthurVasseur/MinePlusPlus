//
// Created by arthu on 24/09/2022.
//

#ifndef MINEPLUSPLUS_STREAMOUTPUT_HPP
#define MINEPLUSPLUS_STREAMOUTPUT_HPP

#include <iostream>
#include <istream>
#include "Types.hpp"

namespace MinePP
{
	std::ostream& operator<<(std::ostream& os, const Byte& byte);
	std::ostream& operator<<(std::ostream& os, const UByte& byte);
	std::ostream& operator<<(std::ostream& os, const Buffer& buffer);
	std::ostream& operator<<(std::ostream& os, const String& string);
	std::ostream& operator<<(std::ostream& os, std::istream& is);
} // MinePP

#endif //MINEPLUSPLUS_STREAMOUTPUT_HPP
