//
// Created by arthur on 15/10/2022.
//

#ifndef MINEPLUSPLUS_MEMORYSTREAM_HPP
#define MINEPLUSPLUS_MEMORYSTREAM_HPP

#include <streambuf>
#include <cstddef>
#include <span>

namespace MinePP
{
	struct MemoryStream : public std::streambuf
	{
	public:
		MemoryStream(std::span<Byte> buffer)
		{
			char* p = reinterpret_cast<char*>(buffer.data());
			this->setg(p, p, p + buffer.size());
		}
	};

	struct IMemoryStream : public virtual MemoryStream, public std::istream
	{
	public:
		IMemoryStream(const char* data, std::size_t size) : MemoryStream(std::span<Byte>((Byte*)data, size)),
															std::istream (static_cast<std::streambuf *>(this))
		{
		}
		IMemoryStream(const std::span<Byte> buffer) : MemoryStream(buffer),
													  std::istream (static_cast<std::streambuf *>(this))
		{
		}
	};
}
#endif //MINEPLUSPLUS_MEMORYSTREAM_HPP
