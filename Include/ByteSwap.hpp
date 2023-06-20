//
// Created by arthur on 20/10/2022.
//

#ifndef MINEPLUSPLUS_BYTESWAP_HPP
#define MINEPLUSPLUS_BYTESWAP_HPP

#include <cstdint>

namespace Internal
{
#if defined(_MSC_VER)

	inline std::uint64_t bswap64(std::uint64_t v) noexcept
	{
		return _byteswap_uint64(v);
	}

	inline std::uint32_t bswap32(std::uint32_t v) noexcept
	{
		return _byteswap_ulong(v);
	}

	inline std::uint16_t bswap16(std::uint16_t v) noexcept
	{
		return _byteswap_ushort(v);
	}

#else
	constexpr auto bswap64(std::uint64_t v) noexcept { return __builtin_bswap64(v); }
	constexpr auto bswap32(std::uint32_t v) noexcept { return __builtin_bswap32(v); }
	constexpr auto bswap16(std::uint16_t v) noexcept { return __builtin_bswap16(v); }
#endif
}

template<typename T>
requires(std::is_integral_v<T>)
inline T ByteSwap(T value) noexcept
{
	if constexpr (std::endian::native == std::endian::big)
		return value;
	else if constexpr (sizeof(T) == 1)
		return value;
	else if constexpr (sizeof(T) == 2)
		return Internal::bswap16(value);
	else if constexpr (sizeof(T) == 4)
		return Internal::bswap32(value);
	else if constexpr (sizeof(T) == 8)
		return Internal::bswap64(value);
	else static_assert(sizeof(T) == 8, "ByteSwap: Unsupported type");
}

#endif //MINEPLUSPLUS_BYTESWAP_HPP
