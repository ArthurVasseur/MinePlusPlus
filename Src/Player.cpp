//
// Created by arthur on 22/09/2022.
//

#include "Player.hpp"
#include "Network/Session.hpp"

namespace MinePP
{

	Player::Player(std::shared_ptr<Network::TCP::Session>& session) : _session(session)
	{

	}

	void Player::HandlePacket(Network::Packet& packet)
	{
//		Network::McPacket mcPacket;
//		mcPacket << packet.buffer;
//		switch (_state)
//		{
//			case State::C_Handshake:
//			{
//				Network::C_Handshake handshake;
//				handshake << mcPacket.data;
//				if (handshake.nextState == 1)
//				{
//					Network::McPacket responseMcPacket;
//					responseMcPacket.id = 0x00;
//					Network::String json;
//					json.value = R"({"version":{"name":"1.19.2","protocol":760},"players":{"max":100,"online":0},"description":{"text":"A MinePlusPlus server"}})";
//					json.size = json.value.size();
//					responseMcPacket.data << json;
//					responseMcPacket.length = responseMcPacket.data.size();
//					Network::Buffer buffer;
//					buffer << responseMcPacket;
//					_session->SendPacket(buffer);
//				}
//				else if (handshake.nextState == 2)
//				{
//
//				}
//				break;
//			}
//		}
//		{
//
//		}
	}

	bool Player::IsConnected() const
	{
		return _session->IsConnected();
	}
}