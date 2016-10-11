#include "Mob.h"

int Mob::mobCount = 0;

Mob::Mob()
{
	this->name = "mob";
	this->description = "A mob rests here.";
	this->combat = false;
	this->enemyPtr = 0;
	this->roundTime = 1700;
	this->lastRound = 0;
	this->hp = 300;
	this->hpMax = 300;
	this->str = 3;
	this->id = Mob::NextMobID();
	this->timer = false;
	this->canmove = true;
	this->canattack = true;
	this->cansee = true;
	this->cantalk = true;
}

int Mob::NextMobID()
{
	return Mob::mobCount++;
}

int Mob::GetID()
{
	return this->id;
}

void Mob::StartTimer(int time, int skillID, int targetID, int type)
{
	this->timer = true;
	this->time = time;
	this->type = type;
	this->skillID = skillID;
	this->targetID = targetID;
	this->start = GetTickCount();
}

bool Mob::CheckTimer()
{
	if(timer)
	{
		if(GetTickCount() - start >= time)
		{
			timer = false;
			return true;
		}
	}
	return false;
}

int Mob::GetTimerSkill()
{
	return this->skillID;
}

int Mob::GetTimerTarget()
{
	return this->targetID;
}

int Mob::GetTimerType()
{
	return this->type;
}

int Mob::GetRoomID()
{
	return this->roomID;
}

std::string Mob::GetName()
{
	return this->name;
}

std::string Mob::GetDescription()
{
	return this->description;
}

void Mob::WarpTo(int roomID)
{
	this->roomID = roomID;
}

void Mob::Engage(Mob *enemy)
{
	this->combat = true;
	this->enemyPtr = enemy;
	this->lastRound = GetTickCount();
}

void Mob::Disengage()
{
	this->combat = false;
	//this->enemyPtr = 0;
	this->lastRound = GetTickCount();
}


bool Mob::IsEngaged()
{
	return this->combat;
}

bool Mob::CheckRound()
{
	if(!this->combat || this->enemyPtr == 0)
	{
		return false;
	}
	if(GetTickCount() - this->lastRound >= roundTime)
	{
		return true;
	}
	return false;
}

std::string HpToStr(int hp)
{
	if(hp < 10)
	{
		return "Critical";
	}
	else if(hp < 30)
	{
		return "Beaten";
	}
	else if(hp < 50)
	{
		return "Battered";
	}
	else if(hp < 70)
	{
		return "Wounded";
	}
	else if(hp < 95)
	{
		return "Scratched";
	}
	else if(hp >= 95)
	{
		return "Healthy";
	}
	return "void";
}

void Mob::Round()
{
	this->lastRound = GetTickCount();
	if(this->roomID != enemyPtr->GetRoomID())
		this->Disengage();
}

Mob* Mob::GetEnemy()
{
	return this->enemyPtr;
}

void Mob::SetHP(int amount)
{
	this->hp = amount;
	this->hpStr = HpToStr(this->hp/3);
}

std::string Mob::GetHPStr()
{
	return this->hpStr;
}

int Mob::GetHP()
{
	return this->hp;
}

int Mob::GetStr()
{
	return this->str;
}
void Mob::SetStr(int str)
{
	this->str = str;
}

void Mob::SetRoundTime(int time)
{
	this->roundTime = time;
}

bool Mob::CanMove() const
{
	return this->canmove;
}

bool Mob::CanAttack() const
{
	return this->canattack;
}

bool Mob::CanSee() const
{
	return this->cansee;
}

bool Mob::CanTalk() const
{
	return this->cantalk;
}

void Mob::SetCanMove(bool can)
{
	this->canmove = can;
}

void Mob::SetCanAttack(bool can)
{
	this->canattack = can;
}

void Mob::SetCanSee(bool can)
{
	this->cansee = can;
}

void Mob::SetCanTalk(bool can)
{
	this->cantalk = can;
}
