#pragma once

#include "Error.h"
#include "Player.h"
class Player;
class Mob;
class Room;
#include "CommandParser.h"
class CommandParser;
#include <winsock.h>
#include <vector>
#include <algorithm>

class Client //a client connected to the server
{
private:
	static const int BUFFER_SIZE = 4096;

	Player* player; //player
	CommandParser* parserPtr;

	SOCKET socket; //client's socket
	sockaddr_in from; //client's addres
	std::string ip; //ip address
	char buffer[BUFFER_SIZE]; //buffer for recv()
	bool connected;
	bool login;

	void OnConnect(); //called when client connects
	void OnDisconnect(); //called when client disconnects
	void OnReceive(std::string message); //called when message is received
public:
	Client(SOCKET socket, sockaddr_in from, CommandParser* parser, std::vector<Room>* roomPtr, std::vector<Mob*>* mobPtr);
	~Client();
	bool operator == (Client a); //needed for std::vector
	
	void CheckReceive(); //check for received messages
	void CheckConditions(); //checks conditions
	void Send(std::string message); //sends message to client
	bool IsConnected();
	SOCKET GetSocket();

	Player* GetPlayer();
};