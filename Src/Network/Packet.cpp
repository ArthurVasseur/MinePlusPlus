//
// Created by arthur on 21/09/2022.
//

#include <stdexcept>
#include <chrono>

#include "Network/Packet.hpp"
#include "Network/Endianness.hpp"

namespace MinePP::Network
{
	static constexpr int SEGMENT_BITS = 0x7F;
	static constexpr int CONTINUE_BIT = 0x80;

	McPacket MakeStatusResponsePacket(const String& json)
	{
		McPacket responseMcPacket;
		responseMcPacket.id = (VarInt)McPacketId::S_StatusResponse;
		responseMcPacket.data << json;
		responseMcPacket.length = responseMcPacket.data.size();
		Buffer buffer;
		buffer << responseMcPacket;
		return responseMcPacket;
	}

	McPacket MakePongPacket(Buffer buffer)
	{
		McPacket pingMcPacket;
		pingMcPacket.id = (VarInt)McPacketId::S_Pong;
		pingMcPacket.length = buffer.size();
		pingMcPacket.data = std::move(buffer);
		return pingMcPacket;
	}

	McPacket MakeDisconnectPacket(const String& reason)
	{
		McPacket disconnectMcPacket;
		disconnectMcPacket.id = (VarInt)McPacketId::S_Kick;
		disconnectMcPacket.data << reason;
		disconnectMcPacket.length = disconnectMcPacket.data.size();
		return disconnectMcPacket;
	}

	McPacket MakeLoginSuccessPacket(const String& username, const UUID& uuid)
	{
		McPacket loginSuccess;
		loginSuccess.id = (VarInt)McPacketId::S_LoginSuccess;
		loginSuccess.data << uuid << username << VarInt(0);
		loginSuccess.length = loginSuccess.data.size();
		return loginSuccess;
	}

	std::string ToString(PacketType packetType)
	{
		switch (packetType)
		{
		case PacketType::Connection:
			return "Connection";
		case PacketType::Disconnection:
			return "Disconnection";
		case PacketType::ReceivedPacket:
			return "ReceivedPacket";
		default:
			return "Unknown";
		}
	}

	Buffer ToVarInt(std::int32_t value)
	{
		Buffer buffer;
		buffer.resize(5);
		std::size_t index = 0;
		while (true)
		{
			if ((value & ~SEGMENT_BITS) == 0)
			{
				buffer[index++] = (Byte)value;
				buffer.resize(index);
				return buffer;
			}
			else
			{
				buffer[index++] = (Byte)(((int)value & SEGMENT_BITS) | CONTINUE_BIT);
				value >>= 7;
			}
		}
		return buffer;
	}

	VarInt ReadVarInt(const Buffer& buffer)
	{
		VarInt varInt = 0;
		Buffer bufferCopy = buffer;
		bufferCopy >> varInt;
		return varInt;
	}

	Buffer& operator>>(Buffer& buffer, bool& b)
	{
		std::copy(buffer.begin(), buffer.begin() + sizeof(bool), reinterpret_cast<Byte*>(&b));
		buffer.erase(buffer.begin(), buffer.begin() + sizeof(bool));
		return buffer;
	}

	Buffer& operator>>(Buffer& buffer, Buffer& other)
	{
		std::copy(buffer.begin(), buffer.begin() + other.capacity(), other.begin());
		buffer.erase(buffer.begin(), buffer.begin() + other.capacity());
		return buffer;
	}


	Buffer& operator>>(Buffer& buffer, Long& l)
	{
		std::copy(buffer.begin(), buffer.begin() + sizeof(Long), reinterpret_cast<Byte*>(&l));
		buffer.erase(buffer.begin(), buffer.begin() + sizeof(Long));
		return buffer;
	}

	Buffer& operator>>(Buffer& buffer, String& string)
	{
		VarInt length;
		buffer >> length;
		string.reserve(length);
		string = { reinterpret_cast<char*>(buffer.data()), static_cast<std::size_t>(length) };
		buffer.erase(buffer.begin(), buffer.begin() + length);
		return buffer;
	}


	Buffer& operator>>(Buffer& buffer, UShort& uShort)
	{
		UShort value = 0;
		std::memcpy(&value, buffer.data(), sizeof(UShort));
		uShort = ntohs(value);
		buffer.erase(buffer.begin(), buffer.begin() + sizeof(UShort));
		return buffer;
	}

	Buffer& operator>>(Buffer& buffer, UUID& uuid)
	{
		std::copy(buffer.begin(), buffer.begin() + uuid.size(), uuid.begin());
		buffer.erase(buffer.begin(), buffer.begin() + uuid.size());
		return buffer;
	}

	Buffer& operator>>(Buffer& buffer, VarInt& varInt)
	{
		VarInt value = 0;
		int shift = 0;
		Byte currentByte;

		do
		{
			currentByte = buffer[0];
			buffer.erase(buffer.begin());
			value |= (currentByte & SEGMENT_BITS) << shift;
			shift += 7;
		} while ((currentByte & CONTINUE_BIT) != 0);
		varInt = value;
		return buffer;
	}

	Buffer& operator>>(Buffer& buffer, Handshake& handshake)
	{
		buffer >> handshake.protocolVersion >> handshake.serverAddress >> handshake.serverPort >> handshake.nextState;
		return buffer;
	}

	Buffer& operator>>(Buffer& buffer, Login& login)
	{
		buffer >> login.username >> login.hasSigData;
		if (login.hasSigData)
		{
			buffer >> login.timestamp >> login.publicKeyLength;
			login.publicKey.resize(login.publicKeyLength);
			buffer >> login.publicKey >> login.signatureLength;
			login.signature.resize(login.signatureLength);
			buffer >> login.signature >> login.hasPlayerUUID;
			if (login.hasPlayerUUID)
				buffer >> login.playerUUID;
		}
		return buffer;
	}

	Buffer& operator>>(Buffer& buffer, McPacket& packet)
	{
		VarInt length;
		VarInt id;
		buffer >> length >> id;
		packet.length = length;
		packet.id = id;
		packet.data = buffer;
		return buffer;
	}

	Buffer& operator>>(Buffer& buffer, State& state)
	{
		VarInt value;
		buffer >> value;
		state = static_cast<State>(value);
		return buffer;
	}

	Buffer& operator<<(Buffer& buffer, const UUID& uuid)
	{
		buffer.insert(buffer.end(), uuid.begin(), uuid.end());
		return buffer;
	}

	Buffer& operator<<(Buffer& buffer, Long longValue)
	{
		buffer.resize(buffer.size() + sizeof(Long));
		std::memcpy(buffer.data(), &longValue, sizeof(Long));
		return buffer;
	}

	Buffer& operator<<(Buffer& buffer, const String& string)
	{
		Buffer varInt = ToVarInt(string.size());
		buffer.reserve(varInt.size() + string.size());
		buffer.insert(buffer.end(), varInt.begin(), varInt.end());
		buffer.insert(buffer.end(), string.begin(), string.end());
		return buffer;
	}

	Buffer& operator<<(Buffer& buffer, VarInt varInt)
	{
		Buffer varIntBuffer = ToVarInt(varInt);
		buffer.insert(buffer.end(), varIntBuffer.begin(), varIntBuffer.end());
		return buffer;
	}

	Buffer& operator<<(Buffer& buffer, const std::vector<Byte>& vector)
	{
		buffer.insert(buffer.end(), vector.begin(), vector.end());
		return buffer;
	}

	Buffer& operator<<(Buffer& buffer, const std::vector<UByte>& vector)
	{
		buffer.insert(buffer.end(), vector.begin(), vector.end());
		return buffer;
	}

	Buffer& operator<<(Buffer& buffer, LoginSuccess& loginSuccess)
	{
		buffer << loginSuccess.uuid << loginSuccess.username;
		return buffer;
	}


	Buffer& operator<<(Buffer& buffer, const McPacket& mcPacket)
	{
		Buffer packetId = ToVarInt(mcPacket.id);
		Buffer length = ToVarInt(mcPacket.length + packetId.size());
		buffer.reserve(mcPacket.data.size() + packetId.size() + length.size());
		buffer.insert(buffer.end(), length.begin(), length.end());
		buffer.insert(buffer.end(), packetId.begin(), packetId.end());
		buffer.insert(buffer.end(), mcPacket.data.begin(), mcPacket.data.end());
		return buffer;
	}
}