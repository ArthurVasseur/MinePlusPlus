//
// Created by arthu on 24/09/2022.
//

#ifndef MINEPLUSPLUS_TYPES_HPP
#define MINEPLUSPLUS_TYPES_HPP

#include <cstddef>
#include <vector>
#include <string>

namespace MinePP
{
	/**
	 * @brief Represents an signed 8-bit integer between -128 and 127
	 */
	using Byte = std::int8_t;

	/**
	 * @brief Represents an unsigned 8-bit integer between 0 and 255
	 */
	using UByte = std::uint8_t;

	/**
	 * @brief Represents an signed 32-bit integer between -2147483648 and 2147483647
	 */
	using VarInt = std::int32_t;

	/**
	 * @brief Represents an signed 64-bit integer between -9223372036854775808 and 9223372036854775807
	 */
	using VarLong = std::int64_t;

	/**
	 * @brief Represents an unsigned 16-bit integer between 0 and 65535
	 */
	using UShort = std::uint16_t;

	/**
	 * @brief Represents an signed 64-bit integer between -9223372036854775808 and 9223372036854775807
	 */
	using Long = std::int64_t;
	/**
	 * @brief Represents a buffer of bytes
	 */
	using Buffer = std::vector<Byte>;

	/**
	 * @brief Represents an array of UTF-8 characters
	 */
	class String : public std::string
	{
		using std::string::string;
	};

	/**
	 * @brief Represents an UUID encoded as a unsigned 128-bit integer
	 */
	using UUID = std::array<UByte, 16>;
}


#endif //MINEPLUSPLUS_TYPES_HPP
