//
// Created by arthur on 22/09/2022.
//

#ifndef MINEPLUSPLUS_PLAYER_HPP
#define MINEPLUSPLUS_PLAYER_HPP
#include <memory>
#include "Network/Packet.hpp"

namespace MinePP
{
	namespace Network::TCP
	{
		class Session;
	}
	class Player
	{
	public:
		explicit Player(std::shared_ptr<Network::TCP::Session> &session);
		Player(const Player& other) = default;
		Player(Player&& other) noexcept = default;
		Player& operator=(const Player& other) = default;

		void HandlePacket(Network::Packet& packet);

		bool IsConnected() const;

	private:
		std::shared_ptr<Network::TCP::Session> _session;
	};
} // MinePP
#endif //MINEPLUSPLUS_PLAYER_HPP
