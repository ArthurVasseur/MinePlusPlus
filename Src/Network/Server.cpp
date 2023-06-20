//
// Created by arthur on 21/09/2022.
//

#include "Network/Server.hpp"

namespace MinePP::Network::TCP
{
	Server::Server(asio::io_context& io_context, short port) : _acceptor(io_context,
			asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port)), _availablePackets(), _maxId(0)
	{
		DoAccept();
	}

	void Server::DoAccept()
	{
		_acceptor.async_accept(
				[this](std::error_code ec, asio::ip::tcp::socket socket)
				{
					if (!ec)
					{
						auto session = std::make_shared<Session>(std::move(socket), _availablePackets, ++_maxId);
						_sessions.push_back(session);
						session->Start();
						Packet packet;
						packet.type = PacketType::Connection;
						packet.session = session;
						_availablePackets.PushBack(std::move(packet));
						DoAccept();
					}
				});
	}

	std::pair<Packet, bool> Server::GetPacket()
	{
		return _availablePackets.PopFront();
	}

	void Server::RemoveSession(std::shared_ptr<Session> &session)
	{
		_sessions.erase(std::remove(_sessions.begin(), _sessions.end(), session), _sessions.end());
	}
} // MinePP::Network::TCP