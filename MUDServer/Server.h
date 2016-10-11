#pragma once

#include <fstream>
#include <winsock.h>
#pragma comment(lib, "ws2_32.lib")

#include "Error.h"
#include "Client.h"
#include "Skill.h"
class Client;
class Room;
#include "CommandParser.h"
class CommandParser;
#include <list>
#include <vector>

//#include "lua.hpp"

#pragma comment(lib, "lua5.1.lib")

class Server //tcp server
{
private:
	//network stuff
	const static int WS_VERSION_HI = 2; //high-order byte (minor version number)
	const static int WS_VERSION_LO = 2; //low-order byte (major version number)

	WSAData wsaData; //winsock data
	SOCKET server; //socket
	sockaddr_in local; //socket address
	fd_set readfds; //asynchronous stuff

	std::list<Client*> clients; //connected clients

	void BuildSocket(int port);
	void BeginHosting();
	void RegisterLuaFunc();

	//game stuff 
	lua_State* lua; //lua_Stae
	std::vector<Room> rooms;
	std::vector<Mob*> mobs;
	std::vector<Skill> skills;
	CommandParser* parser;
public:
	const static int WS_DEFAULTPORT = 5374; //default port used for connections
	Server(void);
	~Server(void);

	std::list<Client*>* GetClients();
	std::vector<Mob*>* GetMobs();
	std::vector<Skill>* GetSkills();
	std::vector<Room>* GetRooms();
	lua_State* GetLua();

	void OnClientConnect(); //Must be thread-safe
	void Host(); //Hosts a server on the default port
	void Host(int port); //hosts a server on the specified port

	Mob* FindMob(const std::string& name, Player* player);

	void BackUp(const std::string &oldfile);
	void SaveRooms(const std::string& file);
	void LoadRooms(const std::string& file);
	void SaveSkills(const std::string& file);
	void LoadSkills(const std::string& file);
};

int AcceptThread(void* data); //thread for accepting new clients
