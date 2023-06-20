//
// Created by arthur on 21/09/2022.
//

#ifndef MINEPLUSPLUS_SESSION_HPP
#define MINEPLUSPLUS_SESSION_HPP

#include <memory>
#include <string>
#include <iostream>

#include <asio.hpp>
#include "Packet.hpp"
#include "Threading/SafeDeque.hpp"
#include "CircularBuffer.hpp"

namespace MinePP::Network::TCP
{
	class Session : public std::enable_shared_from_this<Session>
	{
	public:
		explicit Session(asio::ip::tcp::socket socket, Threading::SafeDeque<Packet>& availablePackets, UShort id);

		Session(const Session&) = delete;

		Session(Session&&) noexcept = default;

		Session& operator=(const Session&) = delete;

		void Start();

		const std::string& GetAddress() const;

		bool IsConnected() const;

		UShort GetId() const;

		State GetState() const
		{
			return _state;
		}

		void SetState(State state)
		{
			_state = state;
		}

		Session& SendPacket(const McPacket& packet)
		{
			Buffer buffer;
			buffer << packet;
//			asio::async_write(_socket, asio::buffer(buffer),
//					[this](std::error_code ec, std::size_t length)
//					{
//						if (ec)
//						{
//							std::cerr << "Error while sending packet" << std::endl;
//						}
//					});
			_socket.async_write_some(asio::buffer(buffer),
					[this](std::error_code ec, std::size_t length)
					{
						if (ec)
						{
							std::cerr << "Error while sending packet" << std::endl;
						}
					});
			return *this;
		}

		void Close();

	private:
		void DoRead();

		Packet MakePacket(PacketType type);

		asio::ip::tcp::socket _socket;
		Buffer _buffer;
		CircularBuffer<Byte, 1024> _circularBuffer;
		Threading::SafeDeque<Packet>& _availablePackets;
		std::string _address;
		UShort _id;
		State _state;
	};
} // MinePP::Network::TCP

#endif //MINEPLUSPLUS_SESSION_HPP
