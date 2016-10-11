#include "Room.h"

int Room::roomCount = 0;

Room::Room()
{
	this->title = "The Void";
	this->description = "You are in the Void. Blackness surrounds you for miles.";
	this->exits = "";
	this->roomID = 0;
	for(int i = 0; i < DIRECTION_COUNT; i++)
	{
		links[i] = -1;
	}
}

Room::Room(const std::string& title, const std::string& description, int ID, std::vector<Mob*>* mobPtr)
{
	this->SetRoom(title, description, ID, mobPtr);
	for(int i = 0; i < DIRECTION_COUNT; i++)
	{
		links[i] = -1;
	}
}

void Room::SetRoom(const std::string& title, const std::string& description, int ID, std::vector<Mob*>* mobPtr)
{
	this->title = title;
	this->description = description;
	this->roomID = ID;
	this->mobPtr = mobPtr;
}

void Room::LinkTo(Direction dir, const Room& link)
{
	this->LinkTo(dir, link.GetID());
	this->exits = (links[NORTH] == -1) ? "" : "N ";
	this->exits += (links[EAST] == -1) ? "" : "E ";
	this->exits += (links[SOUTH] == -1) ? "" : "S ";
	this->exits += (links[WEST] == -1) ? "" : "W ";
	this->exits += (links[UP] == -1) ? "" : "U ";
	this->exits += (links[DOWN] == -1) ? "" : "D ";
}

void Room::LinkTo(Direction dir, int linkID)
{
	this->links[dir] = linkID;
	//update exits string
	this->exits = (links[NORTH] == -1) ? "" : "N ";
	this->exits += (links[EAST] == -1) ? "" : "E ";
	this->exits += (links[SOUTH] == -1) ? "" : "S ";
	this->exits += (links[WEST] == -1) ? "" : "W ";
	this->exits += (links[UP] == -1) ? "" : "U ";
	this->exits += (links[DOWN] == -1) ? "" : "D ";
}

int Room::GetID() const
{
	return this->roomID;
}

int Room::GetLink(Direction dir) const
{
	return this->links[dir];
}

std::string Room::GetDisplay(const Player& player) const
{
	std::string displayString = "";

	//change color to cyan for title
	Color::ChangeColor(displayString, LIGHTCYAN);
	//add title
	displayString += this->title;

	//add room id for debug
	displayString += "[ID: ";
	displayString += IntToStr(this->roomID);
	displayString += "]";

	displayString += '\n';

	//change color to white for description
	Color::ChangeColor(displayString, WHITE);
	//add description
	displayString += this->description;
	displayString += '\n';
	//add exits
	displayString += "[ obvious exits: ";
	displayString += this->exits;
	displayString += "]\n";
	//add items
	//add mobs
	Color::ChangeColor(displayString, YELLOW);
	int count = 0;
	for(unsigned int i = 0; i < mobPtr->size(); i++)
	{
		if((*mobPtr)[i]->GetRoomID() == this->roomID && (*mobPtr)[i] != (Mob*)&player)
		{
			displayString += "[";
			displayString += IntToStr(count);
			displayString += "]";
			displayString += (*mobPtr)[i]->GetDescription();
			displayString += '\n';
			count++;
		}
	}
	
	//back to white
	Color::ChangeColor(displayString, WHITE);
	//return the string
	return displayString;
}

void Room::SetTitle(const std::string& title)
{
	this->title = title;
}

void Room::SetDescription(const std::string &desc)
{
	this->description = desc;
}

int Room::NextRoomID()
{
	return Room::roomCount++;
}

roomSave Room::ToRoomSave()
{
	roomSave ret;
	sprintf(ret.title, "%s\0", this->title.c_str());
	sprintf(ret.description, "%s\0", this->description.c_str());
	for(int i = 0; i < DIRECTION_COUNT; i++)
	{
		ret.links[i] = this->GetLink((Direction)i);
	}
	ret.roomID = this->roomID;
	return ret;
}

void Room::FromRoomSave(const roomSave& save, std::vector<Mob*>* mobPtr)
{
	this->title = save.title;
	this->description = save.description;
	for(int i = 0; i < DIRECTION_COUNT; i++)
	{
		this->LinkTo((Direction)i, save.links[i]);
	}
	this->roomID = save.roomID;
	this->mobPtr = mobPtr;
}

//combat
std::string DmgToStr(int dmg)
{
	if(dmg < 15)
	{
		return "softly";
	}
	if(dmg < 25)
	{
		return "hard";
	}
	if(dmg < 40)
	{
		return "very hard";
	}
	if(dmg < 50)
	{
		return "extremely hard";
	}
	if(dmg >= 50)
	{
		return "into bloody fragments";
	}
	return "";
}

void Room::CombatRound(Mob* mob, Server* serverPtr)
{
	int damage = 5 + (rand()%50) + mob->GetStr();
	mob->GetEnemy()->SetHP(mob->GetEnemy()->GetHP() - damage);
	
	for(std::list<Client*>::iterator it = serverPtr->GetClients()->begin(); it != serverPtr->GetClients()->end(); it++)
	{
		if((*it)->GetPlayer()->GetRoomID() == mob->GetRoomID())
		{
			std::string message = "\n";
			if((*it)->GetPlayer() == (Player*)mob)
			{
				Color::ChangeColor(message, GREEN);
				message += "You";
				message += " strike ";
			}
			else
			{
				if(mob->GetEnemy() == (Mob*)(*it)->GetPlayer())
				{
					Color::ChangeColor(message, RED);
				}
				message += mob->GetName();
				message += " strikes ";
			}
			
			if(mob->GetEnemy() == (Mob*)(*it)->GetPlayer())
			{
				message += "your body ";
			}
			else
			{
				message += mob->GetEnemy()->GetName();
				message += "'s body ";
			}
			message += DmgToStr(damage);
			message += "!\n";
			Color::ChangeColor(message, WHITE);
			(*it)->Send(message);
			(*it)->Send((*it)->GetPlayer()->GetPrompt());
		}
	}
	mob->Round();
}

void Room::CheckDeath(Mob* mob, Server* serverPtr)
{
	//check for death
	if(mob->GetHP() <= 0)
	{
		for(std::list<Client*>::iterator it = serverPtr->GetClients()->begin(); it != serverPtr->GetClients()->end(); it++)
		{
			if((*it)->GetPlayer()->GetRoomID() == mob->GetRoomID())
			{
				std::string message = "\n";
				Color::ChangeColor(message, CYAN);
				message += mob->GetName();
				message += " has died!\n";
				Color::ChangeColor(message, WHITE);
				(*it)->Send(message);
				(*it)->Send((*it)->GetPlayer()->GetPrompt());
				mob->WarpTo(1);
				mob->SetHP(300);
				mob->Disengage();
				//make all mobs fighting this one disengage aswell
				for(int i = 0; i < serverPtr->GetMobs()->size(); i++)
				{
					if((*serverPtr->GetMobs())[i]->GetEnemy() == mob)
					{
						(*serverPtr->GetMobs())[i]->Disengage();
					}
				}
				if((*it)->GetPlayer() == (Player*)mob)
				{
					(*it)->Send((*it)->GetPlayer()->GetCurrentRoomDisplay());
					(*it)->Send((*it)->GetPlayer()->GetPrompt());
				}
			}
		}
	}
}

void Room::CheckFights(Server* serverPtr)
{
	for(int i = 0; i < serverPtr->GetMobs()->size(); i++)
	{
		if((*serverPtr->GetMobs())[i]->IsEngaged() && (*serverPtr->GetMobs())[i]->CheckRound())
		{
			if((*serverPtr->GetMobs())[i]->CanAttack())
			{
				CombatRound((*serverPtr->GetMobs())[i], serverPtr);
			}
		}
		CheckDeath((*serverPtr->GetMobs())[i], serverPtr);
	}
}
//direction stuffs
Direction DirReverse(Direction dir)
{
	switch(dir)
	{
	case NORTH:
		return SOUTH;
	case SOUTH:
		return SOUTH;
	case EAST:
		return WEST;
	case WEST:
		return EAST;
	case DOWN:
		return UP;
	case UP:
		return DOWN;
	default:
		return DIRECTION_COUNT;
	}
}

std::string DirToString(Direction dir)
{
	switch(dir)
	{
	case NORTH:
		return "north";
	case SOUTH:
		return "south";
	case EAST:
		return "east";
	case WEST:
		return "west";
	case DOWN:
		return "down";
	case UP:
		return "up";
	default:
		return "void";
	}
}

Direction StringToDir(std::string str)
{
	if(str == "n" || str == "north")
		return NORTH;
	else if(str == "s" || str == "south")
		return SOUTH;
	else if(str == "e" || str == "east")
		return EAST;
	else if(str == "w" || str == "west")
		return WEST;
	else if(str == "d" || str == "down")
		return DOWN;
	else if(str == "u" || str == "up")
		return UP;
	return DIRECTION_COUNT;
}