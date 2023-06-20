//
// Created by arthur on 21/09/2022.
//

#ifndef MINEPLUSPLUS_GAMESERVER_HPP
#define MINEPLUSPLUS_GAMESERVER_HPP

#include <unordered_map>
#include <cryptopp/rsa.h>

#include "Network/Server.hpp"
#include "Threading/SafeDeque.hpp"
#include "Player.hpp"
#include "SparseArray.hpp"
#include "PacketHandler.hpp"
#include "NBT/NBT.hpp"

namespace MinePP
{
	namespace Network::TCP
	{
		class Session;
	}
	class GameServer
	{
	public:
		explicit GameServer(short port);

		~GameServer() = default;

		void Run();

		const std::vector<UByte>& GetDERPublicKey() const;

		Player& GetPlayerFromSession(std::shared_ptr<Network::TCP::Session>& session);

	private:
		void HandlePacket(Network::Packet& packet);

		asio::io_context _ioContext;
		Network::TCP::Server _host;
		SparseArray<Player> _sessions; // session id as index
		PacketHandler _packetHandler;
		CryptoPP::RSA::PrivateKey _privateKey;
		CryptoPP::RSA::PublicKey _publicKey;
		std::vector<UByte> _derPublicKey;
	};
} // MinePP

#endif //MINEPLUSPLUS_GAMESERVER_HPP
