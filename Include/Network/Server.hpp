//
// Created by arthur on 21/09/2022.
//


#ifndef MINEPLUSPLUS_SERVER_HPP
#define MINEPLUSPLUS_SERVER_HPP

#include <memory>
#include <iostream>
#include <vector>

#include <asio.hpp>
#include "Packet.hpp"
#include "Session.hpp"
#include "Threading/SafeDeque.hpp"

namespace MinePP::Network::TCP
{
	class Server
	{
	public:
		Server(asio::io_context& io_context, short port);
		~Server() = default;
		std::pair<Packet, bool> GetPacket();
		void RemoveSession(std::shared_ptr<Session> &session);
	private:
		void DoAccept();
		Threading::SafeDeque<Network::Packet> _availablePackets;
		asio::ip::tcp::acceptor _acceptor;
		std::vector<std::shared_ptr<Session>> _sessions;
		UShort _maxId;
	};
}

#endif //MINEPLUSPLUS_SERVER_HPP
