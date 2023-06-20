#include <fstream>
#include <algorithm>
#include <istream>
#include <unordered_map>
#include "Network/Server.hpp"
#include "GameServer.hpp"
#include "Logger.hpp"
#include "Utils/ZLib.hpp"
#include "NBT/NBT.hpp"
#include "MemoryStream.hpp"

int main()
{
	using namespace MinePP;
	try
	{
		GameServer server(25565);
		server.Run();
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}
}