//
// Created by arthur on 21/09/2022.
//

#include <format>
#include <cryptopp/osrng.h>
#include "GameServer.hpp"
#include "Logger.hpp"

namespace MinePP
{
	GameServer::GameServer(short port)
			: _ioContext(), _host(_ioContext, port), _packetHandler(*this), _derPublicKey(8192)
	{
		CryptoPP::InvertibleRSAFunction params;
		CryptoPP::DefaultAutoSeededRNG rng{};
		params.GenerateRandomWithKeySize(rng, 1024);
		_privateKey = CryptoPP::RSA::PrivateKey(params);
		_publicKey = CryptoPP::RSA::PublicKey(params);
		CryptoPP::ArraySink sink(_derPublicKey.data(), _derPublicKey.size());
		_publicKey.DEREncodePublicKey(sink);
	}

	void GameServer::Run()
	{
		while (true)
		{
			if (_ioContext.stopped())
				_ioContext.restart();
			_ioContext.poll();
			auto [packet, success] = _host.GetPacket();
			if (success)
			{
				HandlePacket(packet);
			}
		}
	}

	void GameServer::HandlePacket(Network::Packet& packet)
	{
		switch (packet.type)
		{
		case Network::PacketType::Connection:
		{
			Logger::Info("New connection from " + packet.session->GetAddress());
			_sessions.Emplace(packet.session->GetId(), packet.session);
			break;
		}
		case Network::PacketType::Disconnection:
		{
			Logger::Info("Disconnection from " + packet.session->GetAddress());
			_host.RemoveSession(packet.session);
			_sessions.Erase(packet.session->GetId());
			break;
		}
		case Network::PacketType::ReceivedPacket:
		{
			Logger::Info("Received packet from " + packet.session->GetAddress());
			Logger::Debug(packet.buffer);
			_packetHandler.HandlePacket(packet);
			break;
		}
		default:
		{
			std::cout << "Unknown packet from " << packet.session->GetAddress() << std::endl;
			break;
		}
		}
	}

	const std::vector<UByte>& GameServer::GetDERPublicKey() const
	{
		return _derPublicKey;
	}

	Player& GameServer::GetPlayerFromSession(std::shared_ptr<Network::TCP::Session>& session)
	{
		return _sessions[session->GetId()];
	}
} // MinePP