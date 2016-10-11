#include "Player.h"

Player::Player()
{
	this->mobPtr = 0;
	this->name = "shade";
	this->description = "A Shade is here. Someone failed while making this server...";
	this->imm = false;
	this->SetHP(300);
}

Player::Player(std::vector<Room>* roomPtr, std::vector<Mob*>* mobPtr)
{
	this->description = "A man stands here.";
	this->name = "man";
	this->hp = rand();
	this->roomPtr = roomPtr;
	this->mobPtr = mobPtr;
	this->roomID = 0;
	this->imm = false;
	this->SetHP(300);
	mobPtr->push_back((Mob*)this);
}

Player::~Player(void)
{
	this->roomID = 0;
	this->name = "linklessd";
	for(std::vector<Mob*>::iterator it = mobPtr->begin(); it != mobPtr->end(); it++)
	{
		if((*it) == (Mob*)this)
		{
			mobPtr->erase(it);
			break;
		}
	}
}

void Player::Update()
{
}

bool Player::MovePlayer(Direction dir, std::list<Client*>* clientsPtr)
{
	if((*roomPtr)[roomID].GetLink(dir) != -1)
	{
		for(std::list<Client*>::iterator it = clientsPtr->begin(); it != clientsPtr->end(); it++)
		{
			if((*it)->GetPlayer()->GetRoomID() == this->GetRoomID() && (*it)->GetPlayer() != this)
			{
				std::string buffer = "\n\n";
				buffer += this->name;
				buffer += " leaves ";
				buffer += DirToString(dir);
				buffer += ".\n";
				(*it)->Send(buffer);
				(*it)->Send((*it)->GetPlayer()->GetPrompt());
			}
		}
		this->roomID = (*roomPtr)[roomID].GetLink(dir);
		this->Disengage();
		for(std::list<Client*>::iterator it = clientsPtr->begin(); it != clientsPtr->end(); it++)
		{
			if((*it)->GetPlayer()->GetRoomID() == this->GetRoomID() && (*it)->GetPlayer() != this)
			{
				std::string buffer = "\n\n";
				buffer += this->name;
				buffer += " has arrived from the ";
				buffer += DirToString(DirReverse(dir));
				buffer += ".\n";
				(*it)->Send(buffer);
				(*it)->Send((*it)->GetPlayer()->GetPrompt());
			}
		}
		return true;
	}
	else
	{
		return false;
	}
}


void Player::Say(const std::string& msg, std::list<Client*>* clientsPtr)
{
	for(std::list<Client*>::iterator it = clientsPtr->begin(); it != clientsPtr->end(); it++)
	{
		if((*it)->GetPlayer()->GetRoomID() == this->GetRoomID() && (*it)->GetPlayer() != this)
		{
			std::string buffer = "\n";
			buffer += this->name;
			buffer += " says '";
			buffer += msg;
			buffer += "'\n";
			(*it)->Send(buffer);
			(*it)->Send((*it)->GetPlayer()->GetPrompt());
		}
	}
}

void Player::Yell(const std::string& msg, std::list<Client*>* clientsPtr)
{
	for(std::list<Client*>::iterator it = clientsPtr->begin(); it != clientsPtr->end(); it++)
	{
		if((*it)->GetPlayer() != this)
		{
			std::string buffer = "\n";
			buffer += this->name;
			buffer += " yells '";
			buffer += msg;
			buffer += "'\n";
			(*it)->Send(buffer);
			(*it)->Send((*it)->GetPlayer()->GetPrompt());
		}
	}
}


void Player::Emote(const std::string& msg, std::list<Client*>* clientsPtr)
{
	for(std::list<Client*>::iterator it = clientsPtr->begin(); it != clientsPtr->end(); it++)
	{
		if((*it)->GetPlayer()->GetRoomID() == this->GetRoomID() && (*it)->GetPlayer() != this)
		{
			std::string buffer = "\n";
			buffer += this->name;
			buffer += " ";
			buffer += msg;
			buffer += "\n";
			(*it)->Send(buffer);
			(*it)->Send((*it)->GetPlayer()->GetPrompt());
		}
	}
}

std::string Player::GetPrompt() const
{
	std::string prompt = "\n* HP:";
	prompt += this->hpStr;
	prompt += " SP:";
	prompt += (imm) ? "Endless" : "Bursting";
	prompt += " MV:";
	prompt += (imm) ? "Endless" : "Strong";
	if(this->combat)
	{
		prompt += " - ";
		prompt += this->enemyPtr->GetName();
		prompt += ": ";
		prompt += this->enemyPtr->GetHPStr();
	}
	prompt += " > ";
	return prompt;
}

std::string Player::GetCurrentRoomDisplay() const
{
	if(this->CanSee())
	{
		return (*roomPtr)[roomID].GetDisplay((*this));
	}
	else
	{
		return "\nYou can't see anything!\n";
	}
}

bool Player::IsFighting(std::list<Client*>* clientsPtr)
{
	for(std::list<Client*>::iterator it = clientsPtr->begin(); it != clientsPtr->end(); it++)
	{
		if((*it)->GetPlayer()->GetEnemy() == (Mob*)this && (*it)->GetPlayer()->IsEngaged())
		{
			return true;
		}
	}
	return false;
}

bool Player::IsImm()
{
	return this->imm;
}
void Player::ToggleImm()
{
	this->imm = !this->imm;
}

void Player::SetName(const std::string& name)
{
	this->name = name;
	this->description = name;
	this->description += " stands here.";
}