#pragma once

#include "Client.h"
class Client;
#include "Room.h"
class Room;
enum Direction;
#include "Mob.h"
#include <string>
#include <vector>
#include <list>


class Player : public Mob
{
public:
	//statistics
	bool imm;

	std::vector<Room>* roomPtr; //pointer to room vector
	std::vector<Mob*>* mobPtr; //pointer to mob vector
public:
	Player();
	Player(std::vector<Room>* roomPtr, std::vector<Mob*>* mobPtr);
	~Player();

	void Update();

	bool MovePlayer(Direction dir, std::list<Client*>* clientsPtr);
	void Say(const std::string& msg, std::list<Client*>* clientsPtr);
	void Yell(const std::string& msg, std::list<Client*>* clientsPtr);
	void Emote(const std::string& msg, std::list<Client*>* clientsPtr);

	bool IsImm();
	bool IsFighting(std::list<Client*>* clientsPtr);
	void ToggleImm(); //toggles immortal status

	std::string GetPrompt() const; //returns prompt
	std::string GetCurrentRoomDisplay() const; //return display for room player is in

	void SetName(const std::string& name);
};
