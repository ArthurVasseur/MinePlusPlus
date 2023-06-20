//
// Created by arthur on 21/09/2022.
//

#ifndef MINEPLUSPLUS_PACKET_HPP
#define MINEPLUSPLUS_PACKET_HPP

#include <memory>

#include "Types.hpp"
#include "CircularBuffer.hpp"

namespace MinePP::Network
{

	namespace TCP
	{
		class Session;
	}

	enum PacketType
	{
		Connection,
		Disconnection,
		ReceivedPacket
	};

	enum class State : VarInt
	{
		Handshake = 0,
		Status = 1,
		Login = 2,
		Play = 3
	};

	enum class McPacketId : VarInt
	{
		// State = Handshake
		C_Handshake = 0x00,
		// State = Status
		S_StatusResponse = 0x00,
		C_StatusResponse = 0x00,
		C_Ping = 0x01,
		S_Pong = 0x01,
		// State = C_Login
		C_Login = 0x00,
		S_EncryptionRequest = 0x01,
		S_LoginSuccess = 0x02,
		S_Kick = 0x00,

	};

	struct Packet
	{
		PacketType type;
		std::shared_ptr<TCP::Session> session;
		Buffer buffer;
	};

	struct McPacket
	{
		VarInt length;
		VarInt id;
		Buffer data;
	};


	struct Handshake
	{
		VarInt protocolVersion;
		String serverAddress;
		unsigned short serverPort;
		State nextState;
	};

	struct Login
	{
		String username;
		bool hasSigData;
		Long timestamp;
		VarInt publicKeyLength;
		std::vector<Byte> publicKey;
		VarInt signatureLength;
		std::vector<Byte> signature;
		bool hasPlayerUUID;
		UUID playerUUID;
	};

	struct EncryptionRequest
	{
		String serverId;
		VarInt publicKeyLength;
		std::vector<Byte> publicKey;
		VarInt verifyTokenLength;
		std::vector<Byte> verifyToken;
	};

	struct LoginSuccess
	{
		UUID uuid;
		String username;
	};

	McPacket MakeStatusResponsePacket(const String& json);

	McPacket MakePongPacket(Buffer buffer);

	McPacket MakeDisconnectPacket(const String& reason);

	McPacket MakeLoginSuccessPacket(const String& username, const UUID& uuid);

	std::string ToString(PacketType packetType);

	Buffer ToVarInt(std::int32_t value);

	VarInt ReadVarInt(const Buffer& buffer);

	template<typename T, std::size_t Size>
	VarInt ReadVarInt(const CircularBuffer<T, Size>& buffer)
	{
		VarInt value = 0;
		int shift = 0;
		Byte currentByte;
		std::size_t i = 0;
		do
		{
			if (i >= buffer.size())
				throw std::runtime_error("Buffer is empty");
			currentByte = buffer[i];
			++i;
			value |= (currentByte & 0x7F) << shift;
			shift += 7;
		} while ((currentByte & 0x80) != 0);
		return value;
	}

	template<typename T, std::size_t Size>
	VarInt ReadVarInt(CircularBuffer<T, Size>& buffer)
	{
		VarInt value = 0;
		int shift = 0;
		Byte currentByte;

		do
		{
			if (buffer.empty())
				throw std::runtime_error("Buffer is empty");
			currentByte = buffer.front();
			buffer.pop_front();
			value |= (currentByte & 0x7F) << shift;
			shift += 7;
		} while ((currentByte & 0x80) != 0);
		return value;
	}

	template<typename T, std::size_t BufferSize>
	CircularBuffer<T, BufferSize>& operator>>(CircularBuffer<T, BufferSize>& buffer, VarInt varInt)
	{
		varInt = ReadVarInt(buffer);
		return buffer;
	}

	/**
	 * @brief Copy the content of a CircularBuffer into a Buffer
	 * @tparam T The type of the elements.
	 * @tparam BufferSize The size of the buffer.
	 * @param buffer The buffer to read from.
	 * @param newBuffer The buffer to write to.
	 * @return The buffer shrank by the size of the newBuffer.
	 * @attention The newBuffer must be allocated (.reserve()) before calling this function. Otherwise, the the data will be not copied.
	 */
	template<typename T, std::size_t BufferSize>
	CircularBuffer<T, BufferSize>& operator>>(CircularBuffer<T, BufferSize>& buffer, Buffer& newBuffer)
	{
		for (auto i = 0; i < newBuffer.capacity(); ++i)
		{
			newBuffer.push_back(buffer.front());
			buffer.pop_front();
		}
		return buffer;
	}

//Primitive deserialization
	/**
	 * @param buffer The buffer to read from.
	 * @param other The buffer to write to.
	 * @return The buffer shrank by the size of the other buffer.
	 * @attention The other buffer must be allocated (.reserve()) before calling this function. Otherwise, the the data will be not copied.
	 */
	Buffer& operator>>(Buffer& buffer, bool& b);

	Buffer& operator>>(Buffer& buffer, Buffer& other);

	Buffer& operator>>(Buffer& buffer, Long& l);

	Buffer& operator>>(Buffer& buffer, String& string);

	Buffer& operator>>(Buffer& buffer, UShort& uShort);

	Buffer& operator>>(Buffer& buffer, UUID& uuid);

	Buffer& operator>>(Buffer& buffer, VarInt& varInt);

//Packet deserialization
	Buffer& operator>>(Buffer& buffer, Handshake& handshake);

	Buffer& operator>>(Buffer& buffer, Login& login);

	Buffer& operator>>(Buffer& buffer, McPacket& packet);

	Buffer& operator>>(Buffer& buffer, State& state);

// Primitive serialization
	Buffer& operator<<(Buffer& buffer, const UUID& uuid);

	Buffer& operator<<(Buffer& buffer, Long longValue);

	Buffer& operator<<(Buffer& buffer, const String& string);

	Buffer& operator<<(Buffer& buffer, VarInt varInt);

	Buffer& operator<<(Buffer& buffer, const std::vector<UByte>& vector);

//Packet serialization
	Buffer& operator<<(Buffer& buffer, LoginSuccess& loginSuccess);

	Buffer& operator<<(Buffer& buffer, const McPacket& mcPacket);

}
#endif //MINEPLUSPLUS_PACKET_HPP
