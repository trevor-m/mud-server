#pragma once

#include "Player.h"
class Player;
#include "Server.h"
class Server;
#include <vector>
#include <list>
#include <string>
#include <sstream>

class CommandParser
{
private:
	Server* serverPtr;
public:
	std::string ParseMessage(Player& player, const std::string& message);
	Server* GetServer();
	CommandParser(Server* serverPtr);
	~CommandParser(void);
};