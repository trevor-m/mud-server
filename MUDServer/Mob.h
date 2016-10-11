#pragma once

#include "Color.h"
#include "Stats.h"
#include <string>
#include <vector>
#include <list>
#include <windows.h>

class Mob
{
protected:
	static int mobCount;
	std::string name; //used to target mob
	std::string description; //shown when mob is in room
	std::string exam; //examination
	int id;
	//positon
	int roomID; //roomID mob is in

	//status
	int hp, hpMax; //hit points
	int sp, spMax; //spell points
	int mv, mvMax; //movement points
	int str;
	Stats stats; //stats
	bool canmove, canattack, cansee, cantalk;
	std::string hpStr;

	//skill timers
	bool timer;
	int type;
	int time;
	int skillID;
	int targetID;
	unsigned long start;

	//combat
	int roundTime;
	unsigned long lastRound;
	bool combat;
	Mob* enemyPtr; //enemy the mob is fighting
public:
	static int NextMobID();
	Mob();
	int GetID();

	//skill timers
	void StartTimer(int time, int skillID, int targetID, int type);
	bool CheckTimer();
	int GetTimerSkill();
	int GetTimerTarget();
	int GetTimerType();

	//movement
	int GetRoomID();
	void WarpTo(int roomID);

	//combat
	void Engage(Mob* enemy); //enages on the enemy with the id of enemyID in current room
	void Disengage();
	bool IsEngaged();
	bool CheckRound();
	void Round();
	Mob* GetEnemy();

	//status
	bool CanMove() const;
	bool CanAttack() const;
	bool CanSee() const;
	bool CanTalk() const;
	void SetCanMove(bool can);
	void SetCanAttack(bool can);
	void SetCanSee(bool can);
	void SetCanTalk(bool can);

	//accessors
	std::string GetName();
	std::string GetDescription();
	std::string GetHPStr();
	int GetHP();
	int GetStr();
	void SetStr(int str);
	void SetHP(int amount);
	void SetRoundTime(int time);
};
