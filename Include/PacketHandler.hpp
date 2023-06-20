//
// Created by arthu on 23/09/2022.
//

#ifndef MINEPLUSPLUS_PACKETHANDLER_HPP
#define MINEPLUSPLUS_PACKETHANDLER_HPP

#include <unordered_map>
#include <functional>
#include "Network/Packet.hpp"

namespace MinePP
{
	class GameServer;

	class PacketHandler
	{
	public:
		using SessionPtr = std::shared_ptr<Network::TCP::Session>&;

		PacketHandler(GameServer& server);

		~PacketHandler() = default;

		void HandlePacket(Network::Packet& packet);

	private:
		void HandleHandshake(Network::McPacket& packet, SessionPtr session);

		void HandleServerListPing(Network::McPacket& packet, SessionPtr session);

		void HandlePing(Network::McPacket& packet, SessionPtr session);

		void HandleLogin(Network::McPacket& packet, std::shared_ptr<Network::TCP::Session>& sharedPtr);

		std::unordered_map<Network::State, std::unordered_map<Network::McPacketId, std::function<void(
				Network::McPacket&, SessionPtr)>>> _handlers;

		GameServer& _gameServer;
	};
} // MinePP

#endif //MINEPLUSPLUS_PACKETHANDLER_HPP
