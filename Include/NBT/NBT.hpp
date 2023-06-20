//
// Created by arthu on 12/10/2022.
//

#ifndef MINEPLUSPLUS_NBT_HPP
#define MINEPLUSPLUS_NBT_HPP

#include <cstddef>
#include <vector>
#include <variant>
#include <istream>
#include <type_traits>
#include <unordered_map>
#include <exception>
#include <iostream>
#include "Logger.hpp"
#include "MemoryStream.hpp"
#include "ByteSwap.hpp"

namespace MinePP
{
	class NBT
	{
	public:
		enum TagType : std::uint8_t
		{
			TAG_End = 0, // 0x00
			TAG_Byte = 1, // 0x01
			TAG_Short = 2, // 0x02
			TAG_Int = 3, // 0x03
			TAG_Long = 4, // 0x04
			TAG_Float = 5, // 0x05
			TAG_Double = 6, // 0x06
			TAG_Byte_Array = 7, // 0x07
			TAG_String = 8, // 0x08
			TAG_List = 9, // 0x09
			TAG_Compound = 10, // 0x0A
			TAG_Int_Array = 11, // 0x0B
			TAG_Long_Array = 12 // 0x0C
		};

		using TagEnd = std::nullptr_t;
		using TagByte = std::int8_t;
		using TagShort = std::int16_t;
		using TagUShort = std::uint16_t;
		using TagInt = std::int32_t;
		using TagLong = std::int64_t;
		using TagFloat = float;
		using TagDouble = double;
		using TagByteArray = std::vector<std::uint8_t>;
		using TagString = std::string;
		using TagIntArray = std::vector<TagInt>;
		using TagLongArray = std::vector<TagLong>;
		struct Tag;
		using TagList = std::vector<Tag>;
		using TagCompound = std::unordered_map<TagString, Tag>;

		using Variant = std::variant<TagEnd, TagByte, TagShort, TagInt, TagLong, TagFloat, TagDouble, TagByteArray, TagString, TagIntArray, TagLongArray, TagList, TagCompound>;

		struct Tag : public Variant
		{
			using variant::variant;

			explicit Tag(std::uint8_t type, std::istream& is) : _type(static_cast<TagType>(type))
			{
				Decode(type, is);
			}

			explicit Tag(std::istream& is)
			{
				auto type = Decode<Byte>(is);
				if (type == TAG_Compound)
				{
					auto key = Decode<TagString>(is);
					auto value = Decode<TagCompound>(is);
					*this = TagCompound({ { std::move(key), std::move(value) } });
					_type = TAG_Compound;
					return;
				}
				throw std::invalid_argument("Invalid NBT tag");
			}

			void Encode(std::ostream &os) const
			{
				Encode<Byte>(os, static_cast<Byte>(_type));
				if (_type == TAG_Compound)
				{
					auto& compound = std::get<TagCompound>(*this);
					for (auto& [key, value] : compound)
					{
						Encode<TagString>(os, key);
						value.Encode(os);
					}
					Encode<Byte>(os, TAG_End);
					return;
				}
				throw std::invalid_argument("Invalid NBT tag");
			}

			void Decode(std::uint8_t type, std::istream& is)
			{
				switch (type)
				{
				case TAG_End:
					*this = Decode<TagEnd>(is);
					_type = TAG_End;
					break;
				case TAG_Byte:
					*this = Decode<TagByte>(is);
					_type = TAG_Byte;
					break;
				case TAG_Short:
					*this = Decode<TagShort>(is);
					_type = TAG_Short;
					break;
				case TAG_Int:
					*this = Decode<TagInt>(is);
					_type = TAG_Int;
					break;
				case TAG_Long:
					*this = Decode<TagLong>(is);
					_type = TAG_Long;
					break;
				case TAG_Float:
					*this = Decode<TagFloat>(is);
					_type = TAG_Float;
					break;
				case TAG_Double:
					*this = Decode<TagDouble>(is);
					_type = TAG_Double;
					break;
				case TAG_Byte_Array:
					*this = Decode<TagByteArray>(is);
					_type = TAG_Byte_Array;
					break;
				case TAG_String:
					*this = Decode<TagString>(is);
					_type = TAG_String;
					break;
				case TAG_List:
					*this = Decode<TagList>(is);
					_type = TAG_List;
					break;
				case TAG_Compound:
					*this = Decode<TagCompound>(is);
					_type = TAG_Compound;
					break;
				case TAG_Int_Array:
					*this = Decode<TagIntArray>(is);
					_type = TAG_Int_Array;
					break;
				case TAG_Long_Array:
					*this = Decode<TagLongArray>(is);
					_type = TAG_Long_Array;
					break;
				default:
					throw std::runtime_error("Invalid tag type");
				}
			}

		private:

			template <typename T>
			requires(std::is_integral_v<T>)
			void Encode(std::ostream& os, T value) const
			{
				value = ByteSwap(value);
				os.write(reinterpret_cast<const char*>(&value), sizeof(T));
			}

			template <typename T>
			requires(std::is_floating_point_v<T>)
			void Encode(std::ostream& os, T value) const
			{
				os.write(reinterpret_cast<const char*>(&value), sizeof(T));
			}

			template <typename T>
			requires(std::same_as<T, TagString>)
			void Encode(std::ostream& os, T value) const
			{
				Encode<TagShort>(os,value.size());
				os.write(value.data(), value.size());
			}

			template <typename T>
			requires(std::same_as<T, TagByteArray>)
			void Encode(std::ostream& os, T value) const
			{
				Encode<TagInt>(os, value.size());
				for(const auto &elem : value)
					Encode(os, elem);
			}

			template <typename T>
			requires(std::same_as<T, TagIntArray>)
			void Encode(std::ostream& os, T value) const
			{
				Encode<TagInt>(os, value.size());
				for (const auto& elem : value)
					Encode(os, elem);
			}

			template<typename T>
			requires(std::is_same_v<T, TagEnd>)
			T Decode(std::istream& is)
			{
				is.ignore();
				return T();
			}

			template<typename T>
			requires(std::is_floating_point_v<T>)
			T Decode(std::istream& is)
			{
				T value;
				is.read(reinterpret_cast<char*>(&value), sizeof(T));
				return value;
			}

			template<typename T>
			requires(std::is_integral_v<T>)
			T Decode(std::istream& is)
			{
				T value;
				is.read(reinterpret_cast<char*>(&value), sizeof(T));
				return ByteSwap(value);
			}

			template<typename T>
			requires (std::is_same_v<T, TagByteArray>
			          || std::is_same_v<T, TagIntArray>
					  || std::is_same_v<T, TagLongArray>)
			T Decode(std::istream& is)
			{
				T value;
				auto s = Decode<TagInt>(is);
				assert(s >= 0);
				value.resize(s);
				is.read(reinterpret_cast<char*>(value.data()), value.size());
				return value;
			}


			template<typename T>
			requires (std::is_same_v<T, TagString>)
			T Decode(std::istream& is)
			{
				T value;
				auto s = Decode<TagUShort>(is);
				assert(s >= 0);
				value.resize(s);
				is.read(reinterpret_cast<char*>(value.data()), value.size());
				return value;
			}

			template<typename T>
			requires(std::is_same_v<T, TagList>)
			T Decode(std::istream& is)
			{
				auto type = static_cast<TagType>(Decode<TagInt>(is));
				T value(Decode<TagInt>(is));
				for (auto& tag: value)
				{
					Tag uTag(type, is);
					tag = std::move(uTag);
				}
				return value;
			}

			template<typename T>
			requires(std::is_same_v<T, TagCompound>)
			T Decode(std::istream& is)
			{
				T value;
				while (!is.eof())
				{
					auto type = static_cast<TagType>(Decode<TagByte>(is));
					if (type == TAG_End)
						break;
					if (type == TAG_Compound)
					{
						auto name = Decode<TagString>(is);
						Tag uTag(type, is);
						value[name] = std::move(uTag);
					}
					else {
						auto name = Decode<TagString>(is);
						value[name] = Tag(type, is);
					}
				}
				return value;
			}
			TagType _type = TAG_End;
		};
	};

//	std::ostream& operator<<(std::ostream& os, const NBT::Tag& tag)
//	{
//		tag.Encode(os);
//		return os;
//	}
}

#endif //MINEPLUSPLUS_NBT_HPP
