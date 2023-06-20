//
// Created by arthur on 21/09/2022.
//

#include <iostream>

#include "Network/Session.hpp"
#include "Network/Packet.hpp"
#include "Logger.hpp"
#include "CircularBufferView.hpp"
#include <span>

namespace MinePP::Network::TCP
{
	Session::Session(asio::ip::tcp::socket socket, Threading::SafeDeque<Packet>& availablePackets, UShort id) :
			_socket(std::move(socket)),
			_buffer(),
			_availablePackets(availablePackets),
			_id(id),
			_state(State::Handshake)
	{
		_buffer.resize(1024);
		_address = _socket.remote_endpoint().address().to_string();
	}

	void Session::Start()
	{
		DoRead();
	}

	void Session::DoRead()
	{
		auto self(shared_from_this());
		_socket.async_read_some(asio::buffer(_buffer),
				[this, self](std::error_code ec, std::size_t length)
				{
					if (!ec)
					{
						_circularBuffer.push_back(std::span(_buffer.data(), length));
						while (!_circularBuffer.empty())
						{
							const auto& constBuffer = _circularBuffer;
							VarInt packetLength = ReadVarInt(constBuffer);
							if (packetLength > 0)
							{
								if (_circularBuffer.size() < packetLength)
									break;
								Buffer buffer;
								buffer.reserve(packetLength + (length - packetLength));
								_circularBuffer >> buffer;
								_availablePackets.PushBack(
										Packet{ ReceivedPacket, shared_from_this(), std::move(buffer) });
							}
						}
						DoRead();
					}
					else
					{
						_availablePackets.PushBack(Packet{ Disconnection, shared_from_this(), Buffer{} });
						Close();
					}
				});
	}

	const std::string& Session::GetAddress() const
	{
		return _address;
	}

	bool Session::IsConnected() const
	{
		return _socket.is_open();
	}

	UShort Session::GetId() const
	{
		return _id;
	}

	void Session::Close()
	{
		_socket.close();
	}

	Packet Session::MakePacket(PacketType type)
	{
		auto self(shared_from_this());
		Packet packet;
		packet.session = self;
		packet.type = type;
		return packet;
	}

} // MinePP::Network
