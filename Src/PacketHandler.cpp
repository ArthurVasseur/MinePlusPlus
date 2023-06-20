//
// Created by arthur on 23/09/2022.
//

#include <iostream>
#include <chrono>

#include "PacketHandler.hpp"
#include "Network/Session.hpp"
#include "Logger.hpp"
#include "GameServer.hpp"

namespace MinePP
{
	using namespace Network;

	PacketHandler::PacketHandler(GameServer& server) : _handlers(), _gameServer(server)
	{
		using namespace Network;
		_handlers[State::Handshake][McPacketId::C_Handshake] = [this](McPacket& packet, SessionPtr session)
		{ HandleHandshake(packet, session); };
		_handlers[State::Status][McPacketId::C_StatusResponse] = [this](McPacket& packet, SessionPtr session)
		{ HandleServerListPing(packet, session); };
		_handlers[State::Status][McPacketId::C_Ping] = [this](McPacket& packet, SessionPtr session)
		{ HandlePing(packet, session); };
		_handlers[State::Login][McPacketId::C_Login] = [this](McPacket& packet, SessionPtr session)
		{ HandleLogin(packet, session); };
	}

	void PacketHandler::HandlePacket(Packet& packet)
	{
		McPacket mcPacket;
		packet.buffer >> mcPacket;
		auto it = _handlers.find(packet.session->GetState());
		if (it != _handlers.end())
		{
			auto it2 = it->second.find((McPacketId)mcPacket.id);
			if (it2 != it->second.end())
			{
				it2->second(mcPacket, packet.session);
				return;
			}
		}
		Logger::Info("Unhandled packet: " + std::to_string(mcPacket.id));
	}

	void PacketHandler::HandleHandshake(McPacket& packet, SessionPtr session)
	{
		Handshake handshake;
		packet.data >> handshake;
		session->SetState(handshake.nextState);
		if (handshake.nextState == State::Login)
		{
			Logger::Info("Received handshake for login");
//			session->SendPacket(MakeDisconnectPacket(R"({"text":"C_Login is not supported yet", "blood": true, "extra": []})")).Close();
		}
		else if (handshake.nextState == State::Status)
		{
			Logger::Info("Received handshake for status");
		}
	}

	void PacketHandler::HandleServerListPing(McPacket& packet, SessionPtr session)
	{
		if (session->GetState() != State::Status)
			session->SendPacket(
					MakeDisconnectPacket(R"({"text":"Invalid state", "blood": true, "extra": []})")).Close();
		Logger::Info("Received server list ping");
		String json = R"({"version":{"name":"1.19.2","protocol":760},"players":{"max":100,"online":1},"description":{"text":"A MinePlusPlus server"}})";
		session->SendPacket(MakeStatusResponsePacket(json));
	}

	void PacketHandler::HandlePing(McPacket& packet, SessionPtr session)
	{
		Logger::Info("Received ping");
		McPacket response = MakePongPacket(std::move(packet.data));
		session->SendPacket(response);
	}

	void PacketHandler::HandleLogin(McPacket& packet, SessionPtr& session)
	{
		Logger::Info("Received login");
		Login login;
		packet.data >> login;

//		McPacket encryptionRequest;
//		encryptionRequest.id = (int)McPacketId::S_EncryptionRequest;
//		String serverId;
////		serverId.resize(20);
//		std::vector<UByte> publicKey = _gameServer.GetDERPublicKey();
//		std::vector<UByte> verifyToken;
////		verifyToken.resize(4);
//		encryptionRequest.data << serverId << publicKey << verifyToken;
//		session->SendPacket(encryptionRequest);

		session->SendPacket(MakeLoginSuccessPacket(login.username, login.playerUUID));
		session->SetState(State::Play);
	}

} // MinePP