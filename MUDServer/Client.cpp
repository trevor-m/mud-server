#include "Client.h"

Client::Client(SOCKET socket, sockaddr_in from, CommandParser* parser, std::vector<Room>* roomPtr, std::vector<Mob*>* mobPtr)
{
	this->player = new Player(roomPtr, mobPtr);
	this->connected = false;
	this->socket = socket;
	this->from = from;
	this->parserPtr = parser;
	this->login = true;
	this->ip = inet_ntoa(from.sin_addr);
	this->OnConnect();
}

Client::~Client(void)
{
/*		//disconnect when scope is lost
	player->~Player();
	delete player;
	closesocket(this->socket);*/
}

bool Client::operator ==(Client a)
{
	if(this->connected == a.connected
		&& this->socket == a.socket)
	{
		return true;
	}
	return false;
}

SOCKET Client::GetSocket()
{
	return this->socket;
}

Player* Client::GetPlayer()
{
	return this->player;
}

bool Client::IsConnected()
{
	return this->connected;
}

void Client::OnConnect()
{
	this->connected = true;
	//loop through all ips and see if one is indenticle
	for(std::list<Client*>::iterator it = parserPtr->GetServer()->GetClients()->begin(); it != parserPtr->GetServer()->GetClients()->end(); it++)
	{
		if((*it)->ip == this->ip)
		{
			this->Send("I see you are multiplaying. Have fun!\n");
			//this->OnDisconnect();
			break;
		}
	}
	//welcome message
	this->Send("Welcome, friend! What shall your name be? ");
	login = true;
	//this->OnReceive("look");
}

void Client::OnDisconnect()
{
	std::cout << "Client disconnected! [" << this->player->GetName() << "]" << std::endl;
	player->WarpTo(15);
	player->SetName(player->GetName() + "[linkless]");
	this->ip = "none";
	for(std::vector<Mob*>::iterator it = player->mobPtr->begin(); it != player->mobPtr->end(); it++)
	{
		if((*it) == (Mob*)this)
		{
			player->mobPtr->erase(it);
			break;
		}
	}
	this->connected = false;
	closesocket(socket);
}

void Client::OnReceive(std::string message)
{
	//if it is not -ssetlua
	if(message.substr(0, 9) != "-ssetlua ")
	{
		//cut off \n or \r
		if(message.find("\n") != std::string::npos)
			message.replace(message.find("\n"), 1, "");
		if(message.find("\r") != std::string::npos)
			message.replace(message.find("\r"), 1, "");
	}

	if(login)
	{
		//message.replace(message.find(" "), 1, "\0");
		this->player->SetName(message);
		this->player->WarpTo(1);
		login = false;
		std::cout << "Player at " << this->ip << " is now known as " << message << "." << std::endl;
		message = "look";
	}
	else if(message.substr(0, 9) != "-ssetlua ")
	{
		//make it lowercase
		//std::transform(message.begin(), message.end(), message.begin(), ::tolower);
	}

	std::cout << "[" << player->GetName() <<  "]: " << message << std::endl;
	this->Send(parserPtr->ParseMessage(*player, message));
}

void Client::CheckReceive()
{
	//check for recieved messages
	int len = recv(this->socket, this->buffer, Client::BUFFER_SIZE, 0);
	//check if client disconnected
	if(len < 0 || len >= Client::BUFFER_SIZE)
	{
		if(WSAGetLastError() == WSAEWOULDBLOCK)
			return;
		this->OnDisconnect();
	}
	else
	{
		//make the buffer null-terminated
		buffer[len] = '\0';
		//call onrecieve
		std::string str(buffer, len);

		OnReceive(str);
	}
}

void Client::Send(std::string message)
{
	if(send(this->socket, message.c_str(), message.length(), 0) < 0)
	{
		this->OnDisconnect();
	}
}