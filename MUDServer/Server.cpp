#include "Server.h"

#pragma region luafunc
Server* serverPtr = 0;

int StartTimer(lua_State* L)
{
	int player = (int)lua_tonumber(L, 1);
	int target = (int)lua_tonumber(L, 2);
	int skillID = (int)lua_tonumber(L, 3);
	int time = (int)lua_tonumber(L, 4);
	(*serverPtr->GetMobs())[player]->StartTimer(time, skillID, target, 0);
	return 0;
}

int StartDuration(lua_State* L)
{
	int player = (int)lua_tonumber(L, 1);
	int target = (int)lua_tonumber(L, 2);
	int skillID = (int)lua_tonumber(L, 3);
	int time = (int)lua_tonumber(L, 4);
	(*serverPtr->GetMobs())[player]->StartTimer(time, skillID, target, 1);
	return 0;
}

int GetRoomID(lua_State* L)
{
	int playerid = (int)lua_tonumber(L, 1);
	lua_pushnumber(L, (lua_Number)(*serverPtr->GetMobs())[playerid]->GetRoomID());
	return 1;
}

int GetName(lua_State* L)
{
	int playerid = (int)lua_tonumber(L, 1);
	lua_pushstring(L, (*serverPtr->GetMobs())[playerid]->GetName().c_str());
	return 1;
}

int SendToRoom(lua_State* L)
{
	int roomid = (int)lua_tonumber(L, 1);
	std::string text = (std::string)lua_tostring(L, 2);
	for(std::list<Client*>::iterator it = serverPtr->GetClients()->begin(); it != serverPtr->GetClients()->end(); it++)
	{
		if((*it)->GetPlayer()->GetRoomID() == roomid)
		{
			(*it)->Send("\n"+text+"\n");
			(*it)->Send((*it)->GetPlayer()->GetPrompt());
		}
	}
	return 0;
}

int SendTo(lua_State* L)
{
	int playerid = (int)lua_tonumber(L, 1);
	std::string text = (std::string)lua_tostring(L, 2);
	for(std::list<Client*>::iterator it = serverPtr->GetClients()->begin(); it != serverPtr->GetClients()->end(); it++)
	{
		if((*it)->GetPlayer()->GetID() == playerid)
		{
			(*it)->Send("\n"+text+"\n");
			(*it)->Send((*it)->GetPlayer()->GetPrompt());
		}
	}
	return 0;
}

int SetHealth(lua_State* L)
{
	int playerid = (int)lua_tonumber(L, 1);
	int value = (int)lua_tonumber(L, 2);
	(*serverPtr->GetMobs())[playerid]->SetHP(value);
	return 0;
}

int GetHealth(lua_State* L)
{
	int playerid = (int)lua_tonumber(L, 1);
	lua_pushnumber(L, (lua_Number)(*serverPtr->GetMobs())[playerid]->GetHP());
	return 1;
}

int SetCanMove(lua_State* L)
{
	int playerid = (int)lua_tonumber(L, 1);
	int can = lua_toboolean(L, 2);
	(*serverPtr->GetMobs())[playerid]->SetCanMove(!!can);
	return 0;
}

int SetCanAttack(lua_State* L)
{
	int playerid = (int)lua_tonumber(L, 1);
	int can = lua_toboolean(L, 2);
	(*serverPtr->GetMobs())[playerid]->SetCanAttack(!!can);
	return 0;
}

int SetCanSee(lua_State* L)
{
	int playerid = (int)lua_tonumber(L, 1);
	int can = lua_toboolean(L, 2);
	(*serverPtr->GetMobs())[playerid]->SetCanSee(!!can);
	return 0;
}

int SetCanTalk(lua_State* L)
{
	int playerid = (int)lua_tonumber(L, 1);
	int can = lua_toboolean(L, 2);
	(*serverPtr->GetMobs())[playerid]->SetCanTalk(!!can);
	return 0;
}

int IsImm(lua_State* L)
{
	int playerid = (int)lua_tonumber(L, 1);
	bool isimm;
	for(std::list<Client*>::iterator it = serverPtr->GetClients()->begin(); it != serverPtr->GetClients()->end(); it++)
	{
		if((*it)->GetPlayer()->GetID() == playerid)
		{
			isimm = (*it)->GetPlayer()->IsImm();
		}
	}
	lua_pushboolean(L, (int)isimm);
	return 1;
}
#pragma endregion

void Server::RegisterLuaFunc()
{
	lua_register(lua, "StartTimer", StartTimer);
	lua_register(lua, "StartDuration", StartDuration);
	lua_register(lua, "GetRoomID", GetRoomID);
	lua_register(lua, "GetName", GetName);
	lua_register(lua, "SendToRoom", SendToRoom);
	lua_register(lua, "SendTo", SendTo);
	lua_register(lua, "SetHealth", SetHealth);
	lua_register(lua, "GetHealth", GetHealth);
	lua_register(lua, "SetCanMove", SetCanMove);
	lua_register(lua, "SetCanAttack", SetCanAttack);
	lua_register(lua, "SetCanSee", SetCanSee);
	lua_register(lua, "SetCanTalk", SetCanTalk);
	lua_register(lua, "IsImm", IsImm);
}

Server::Server(void)
{
	serverPtr = this;
	int result = WSAStartup(MAKEWORD(Server::WS_VERSION_HI, Server::WS_VERSION_LO), &wsaData);
	if(result != 0)
	{
		MAKE_ERROR("WSAStartup() failed!");
	}
	this->parser = new CommandParser(this);

	//initialize lua
	lua = lua_open();
	luaL_openlibs(lua);
	RegisterLuaFunc();

	//initialize rand()
	srand(GetTickCount());

	//load rooms
	this->LoadRooms("rooms.mrf");
	//load skills
	this->LoadSkills("skills.msf");
}

Server::~Server(void)
{
	std::cout << "Stopped hosting." << std::endl;
	delete this->parser;
	closesocket(server);
	lua_close(lua);
	WSACleanup();
}

void Server::BuildSocket(int port)
{
	local.sin_family = AF_INET;
	local.sin_addr.s_addr = INADDR_ANY;
	local.sin_port = htons((unsigned short)port); //port to use
	//create the socket
	server = socket(AF_INET, SOCK_STREAM, 0);
	//if socket() failed to create the socket
	if(server == INVALID_SOCKET)
	{
		MAKE_ERROR("socket() failed!");
	}

	//bind the socket with the server
	if(bind(server,(sockaddr*)&local, sizeof(local)) != 0)
	{
		MAKE_ERROR("Server is already being hosted!");
	}
}

void Server::BeginHosting()
{
	//start listening
	if(listen(server, 10) != 0)
	{
		MAKE_ERROR("listen() failed!");
	}
}

void Server::Host()
{
	Server::Host(Server::WS_DEFAULTPORT);
}

void Server::Host(int port)
{
	//build socket
	BuildSocket(port);
	//start listening
	BeginHosting();

	//get time
	SYSTEMTIME time;
	GetLocalTime(&time);
	//message
	std::cout << "Started hosting at " << ((time.wHour > 12)? time.wHour - 12 : time.wHour) << ":" << time.wMinute << ":" << time.wSecond
		<< " on " << time.wMonth << "/" << time.wDay << "/" << time.wYear << std::endl;

	/*//start thread to accept new clients
	//CreateThread(0, 0, (LPTHREAD_START_ROUTINE)AcceptThread, (void*)this, 0, 0);*/

	//enter non-blocking mode
	u_long iMode = 1;
	ioctlsocket(server, FIONBIO, &iMode);

//------------------------------------------
	//main loop
	while(1)
	{
		this->OnClientConnect();
		for(std::list<Client*>::iterator it = clients.begin(); it != clients.end(); it++)
		{
			//check is client is still connected
			if(!(*it)->IsConnected())
			{
				delete (*it);
				clients.remove((*it));
				break;
			}
			//recieve message
			(*it)->CheckReceive();
			//update skill timers
			for(unsigned int i = 0; i < mobs.size(); i++)
			{
				if(mobs[i]->CheckTimer())
				{
					switch(mobs[i]->GetTimerType())
					{
					case 0:
						skills[mobs[i]->GetTimerSkill()].OnCast(mobs[i]->GetID(), mobs[i]->GetTimerTarget());
						break;
					case 1:
						skills[mobs[i]->GetTimerSkill()].OnEnd(mobs[i]->GetID(), mobs[i]->GetTimerTarget());
						break;
					}
				}
			}
		}
		//check fights
		rooms[0].CheckFights(this);
	}
//------------------------------------------
}

void Server::OnClientConnect()
{
	//client data
	SOCKET client;
	sockaddr_in from;
	int fromlen = sizeof(from);
	
	//accept connection
	client = accept(server, (struct sockaddr*)&from, &fromlen);
	if(client == INVALID_SOCKET)
	{
		if(WSAGetLastError() == WSAEWOULDBLOCK)
			return;
	}

	std::cout << "Client at " << inet_ntoa(from.sin_addr) << " connected!" << std::endl;
	u_long iMode = 1;
	ioctlsocket(client, FIONBIO, &iMode);
	this->clients.push_back(new Client(client, from, parser, &rooms, &mobs));
}

std::list<Client*>* Server::GetClients()
{
	return &clients;
}

std::vector<Mob*>* Server::GetMobs()
{
	return &mobs;
}

std::vector<Room>* Server::GetRooms()
{
	return &rooms;
}

std::vector<Skill>* Server::GetSkills()
{
	return &skills;
}

lua_State* Server::GetLua()
{
	return lua;
}

Mob* Server::FindMob(const std::string& name, Player* player)
{
	for(unsigned int i = 0; i < mobs.size(); i++)
	{
		//get name
		std::string mobname = mobs[i]->GetName();
		//conver to lowercase
		std::transform(mobname.begin(), mobname.end(), mobname.begin(), ::tolower);

		//if mob has same name and is in same room
		if(mobname == name && mobs[i]->GetRoomID() == player->GetRoomID())
		{
			return mobs[i];
		}
	}
	return NULL;
}

void Server::BackUp(const std::string &oldfile)
{
	std::fstream ifs("backup.txt", std::ios::in);
	int backupcount = 0;
	ifs >> backupcount;
	ifs.close();
	std::fstream ofs("backup.txt", std::ios::trunc | std::ios::out);
	ofs << (backupcount+1);
	ofs.close();
	std::string file = "backup";
	file += IntToStr(backupcount);
	file += ".mrf";
	rename(oldfile.c_str(), file.c_str());
}

#define TITLE_LENGTH 64
#define DESC_LENGTH 256

struct newRoom
{
	int id;
	char title[TITLE_LENGTH];
	char desc[DESC_LENGTH];
	int links[DIRECTION_COUNT];
};

void Server::SaveRooms(const std::string &file)
{
	//write header
	roomHeader head;
	head.rooms = this->rooms.size();
	std::ofstream ofs(file.c_str(), std::ios::binary | std::ios::trunc);
	ofs.write((char*)&head, sizeof(roomHeader));

	//write rooms
	/*roomSave save;
	for(int i = 0; i < head.rooms; i++)
	{
		save = this->rooms[i].ToRoomSave();
		ofs.write((char*)&save, sizeof(roomSave));
	}*/
	roomSave osave;
	newRoom save;
	for(int i = 0; i < head.rooms; i++)
	{
		osave = this->rooms[i].ToRoomSave();
		save.id = osave.roomID;
		sprintf(save.title, "%s\0", osave.title);
		sprintf(save.desc, "%s\0", osave.description);
		for(int i = 0; i < DIRECTION_COUNT; i++)
			save.links[i] = osave.links[i];

		save.links[SOUTH] = osave.links[EAST];
		save.links[EAST] = osave.links[SOUTH];

		ofs.write((char*)&save, sizeof(newRoom));
	}
}

void Server::LoadRooms(const std::string &file)
{
	//read header
	roomHeader head;
	std::ifstream ifs(file.c_str(), std::ios::binary);
	ifs.read((char*)&head, sizeof(roomHeader));

	//load rooms
	roomSave load;
	for(int i = 0; i < head.rooms; i++)
	{
		//move room counter up one
		Room::NextRoomID();
		//read room data
		ifs.read((char*)&load, sizeof(roomSave));
		Room newroom;
		//deserialize
		newroom.FromRoomSave(load, &mobs);
		rooms.push_back(newroom);
	}
}

void Server::SaveSkills(const std::string &file)
{
	//write header
	skillHeader head;
	head.skills = this->skills.size();
	std::ofstream ofs(file.c_str(), std::ios::binary | std::ios::trunc);
	ofs.write((char*)&head, sizeof(skillHeader));

	//write skills
	skillSave save;
	for(int i = 0; i < head.skills; i++)
	{
		//serialize
		save = this->skills[i].ToSkillSave();
		ofs.write((char*)&save, sizeof(skillSave));
	}
}

void Server::LoadSkills(const std::string &file)
{
	//read header
	skillHeader head;
	std::ifstream ifs(file.c_str(), std::ios::binary);
	ifs.read((char*)&head, sizeof(skillHeader));

	//load skills
	skillSave load;
	for(int i = 0; i < head.skills; i++)
	{
		Skill::NextSkillID();
		ifs.read((char*)&load, sizeof(skillSave));
		Skill newskill;
		newskill.FromSkillSave(load, lua);
		skills.push_back(newskill);
	}
}
