#pragma once

#include "Color.h"
class Mob;
#include "Mob.h"
#include "Server.h"
class Server;
#include "Player.h"
class Player;
#include <string>
#include <vector>
#include <sstream>

enum Direction
{
	NORTH,
	SOUTH,
	EAST,
	WEST,
	UP,
	DOWN,
	DIRECTION_COUNT
};

Direction DirReverse(Direction dir);
std::string DirToString(Direction dir);
Direction StringToDir(std::string str);

struct roomHeader
{
	int rooms;
};

struct roomSave
{
	char title[100]; //title
	char description[512]; //description
	int links[DIRECTION_COUNT]; //links to other rooms
	int roomID; //room's unique ID
};

class Room //a room
{
private:
	static int roomCount;
	std::string title; //title
	std::string description; //description
	std::string exits; //exits (will be set when rooms are linked)
	int links[DIRECTION_COUNT]; //links to other rooms -1 if no link
	//int locks[DIRECTION_COUNT]; //id of the lock in specified direction -1 if none
	int roomID; //room's unique ID
	std::vector<Mob*>* mobPtr;
	void CombatRound(Mob* mob, Server* serverPtr);
	void CheckDeath(Mob* mob, Server* serverPtr);
public:
	static int NextRoomID();
	Room();
	//Room(const Room& room);
	Room(const std::string& title, const std::string& description, int ID, std::vector<Mob*>* mobPtr);

	void SetRoom(const std::string& title, const std::string& description, int ID, std::vector<Mob*>* mobPtr);
	void LinkTo(Direction dir, int linkID); //links this room to room with linkID in direction dir
	void LinkTo(Direction dir, const Room& link); //links this room to link in direction dir

	int GetID() const; //returns id of this room
	int GetLink(Direction dir) const;
	std::string GetDisplay(const Player& player) const;
	void SetTitle(const std::string& title);
	void SetDescription(const std::string& desc);

	void CheckFights(Server* serverPtr); //checks all combat for round

	roomSave ToRoomSave();
	void FromRoomSave(const roomSave& save, std::vector<Mob*>* mobPtr);
};

