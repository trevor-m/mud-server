#include "CommandParser.h"

CommandParser::CommandParser(Server* serverPtr)
{
	this->serverPtr = serverPtr;
}

CommandParser::~CommandParser()
{
}

Server* CommandParser::GetServer()
{
	return this->serverPtr;
}

std::string CommandParser::ParseMessage(Player &player, const std::string &message)
{
	std::string reply;
	
	//check if it is a skill
	for(int i = 0; i < serverPtr->GetSkills()->size(); i++)
	{
		std::string sname = (*serverPtr->GetSkills())[i].GetName();
		std::transform(sname.begin(), sname.end(), sname.begin(), ::tolower);

		if(message.substr(0, sname.length()) == sname)
		{
			Mob* target = serverPtr->FindMob(message.substr(sname.length()+1,message.length()-sname.length()+1), &player);
			if(target == 0)
			{
				reply += "\nYou do not see that here.\n";
			}
			else
			{
				if(player.CanAttack())
				{
					(*serverPtr->GetSkills())[i].OnStart(player.GetID(), target->GetID());
				}
				else
				{
					reply += "\nYou can't do that right now!\n";
				}
			}
		}
	}

	//movement
	if(message == "n" || message == "s" || message == "e" || message == "w" || message == "u" || message == "d")
	{
		if(player.CanMove())
		{
			if(player.IsFighting(serverPtr->GetClients()))
			{
				reply += "\nYou cannot do that while fighting!\n";
			}
			else
			{
				Direction dir = StringToDir(message);
				if(player.MovePlayer(dir, serverPtr->GetClients()))
				{
					reply += player.GetCurrentRoomDisplay();
				}
				else
				{
					reply += "\nYou cannot go that way!\n";
				}
			}
		}
		else
		{
			reply += "\nYou can't move right now!\n";
		}
	}
	//look
	else if(message == "look" || message == "l")
	{
		reply += player.GetCurrentRoomDisplay();
	}
	//say
	else if(message.substr(0, 4) == "say ")
	{
		if(player.CanTalk())
		{
			player.Say(message.substr(4, message.length()-4), serverPtr->GetClients());
			reply += "Ok.\n";
		}
		else
		{
			reply += "\nYou can't talk right now!\n";
		}
	}
	//yell
	else if(message.substr(0, 5) == "yell ")
	{
		if(player.CanTalk())
		{
			player.Yell(message.substr(5, message.length()-5), serverPtr->GetClients());
			reply += "Ok.\n";
		}
		else
		{
			reply += "\nYou can't talk right now!\n";
		}
	}
	//emote
	else if(message.substr(0, 6) == "emote ")
	{
		player.Emote(message.substr(6, message.length()-6), serverPtr->GetClients());
		reply += "Ok.\n";
	}
	//kill
	else if(message.substr(0, 2) == "k ")
	{
		if(player.CanAttack())
		{
			Mob* enemy = serverPtr->FindMob(message.substr(2, message.length()-2), &player);
			if(enemy == 0)
			{
				reply += "\nYou do not see that here.\n";
			}
			else
			{
				reply += "\nYou are now fighting a ";
				reply += enemy->GetName();
				reply += "!\n";
				player.Engage(enemy);
			}
		}
		else
		{
			reply += "\nYou can't attack right now!\n";
		}
	}
	//flee
	else if(message == "flee" || message == "f")
	{
		if(player.CanMove())
		{
			int random = rand()%100;
			if(random <= 30)
			{
				reply += "\nYou could not escape!\n";
			}
			else
			{
				std::vector<Direction> exits;
				for(int i = 0; i < DIRECTION_COUNT; i++)
				{
					if((*serverPtr->GetRooms())[player.GetRoomID()].GetLink((Direction)i) != -1)
					{
						exits.push_back((Direction)i);
					}
				}
				int direction = rand() % exits.size();
				reply += "\nYou flee!\n";
				player.Disengage();
				player.MovePlayer(exits[direction], serverPtr->GetClients());
				reply += player.GetCurrentRoomDisplay();
			}
		}
		else
		{
			reply += "\nYou can't move right now!\n";
		}
	}
	//
	else if(message == "score" || message == "sc" || message == "exp" || message == "stat" || message == "stats")
	{
		reply += "\nYou are a level 1 male channeler.";
	}
	//immortal password
	else if(message == "-imm pigpigpig5")
	{
		player.ToggleImm();
		reply += (player.IsImm()) ? "\nYou are now immortal...\n" : "\nYou return to your mortal form.\n";
	}
	//teleport : -port ID
	else if(message.substr(0, 6) == "-port " && player.IsImm())
	{
		int portID = StrToInt(message.substr(6, message.length()-6));
		//check if room id is valid
		bool valid = false;
		for(unsigned int i = 0; i < player.roomPtr->size(); i++)
		{
			if((*player.roomPtr)[i].GetID() == portID)
			{
				valid = true;
				break;
			}
		}
		if(valid)
		{
			player.WarpTo(portID);
			reply += player.GetCurrentRoomDisplay();
		}
		else
		{
			reply += "\nInvalid room ID.\n";
		}
	}
	//build a room : -r
	else if(message == "-r" && player.IsImm())
	{
		Room newroom = Room("New Room", "A newly built room.", Room::NextRoomID(), player.mobPtr);
		player.roomPtr->push_back(newroom);
		reply += "\nNew room built. ID:";
		reply += IntToStr(newroom.GetID());
		reply += "\n";
	}
	//linking room : -rlink DIR ID
	else if(message.substr(0, 7) == "-rlink " && player.IsImm())
	{
		Direction linkdir = StringToDir(message.substr(7, 1));
		int linkID = StrToInt(message.substr(8, message.length()-8));
		//check if room id is valid
		bool valid = false;
		for(unsigned int i = 0; i < player.roomPtr->size(); i++)
		{
			if((*player.roomPtr)[i].GetID() == linkID || linkID == -1)
			{
				valid = true;
				break;
			}
		}
		if(valid)
		{
			(*player.roomPtr)[player.GetRoomID()].LinkTo(linkdir, linkID);
			reply += "\nLinked ";
			reply += DirToString(linkdir);
			reply += " exit to room #:";
			reply += IntToStr(linkID);
			reply += ".\n";
		}
		else
		{
			reply += "\nInvalid room ID.\n";
		}
	}
	//rename room : -rname NAME
	else if(message.substr(0, 7) == "-rname " && player.IsImm())
	{
		(*player.roomPtr)[player.GetRoomID()].SetTitle(message.substr(7, message.length()-7));
		reply += "\nRoom renamed.\n";
	}
	//rename room : -rdesc DESC
	else if(message.substr(0, 7) == "-rdesc " && player.IsImm())
	{
		(*player.roomPtr)[player.GetRoomID()].SetDescription(message.substr(7, message.length()-7));
		reply += "\nDescription changed.\n";
	}
	//list unsused rooms
	else if(message == "-rlist" && player.IsImm())
	{
		reply += "\nThese rooms have no links leading out:\n";
		reply += "-----------------\n";
		for(int i = 0; i < serverPtr->GetRooms()->size(); i++)
		{
			bool unused = true;
			for(int j = 0; j < DIRECTION_COUNT; j++)
			{
				if((*serverPtr->GetRooms())[i].GetLink((Direction)j) != -1)
				{
					unused = false;
				}
			}
			if(unused)
			{
				reply += "Room #";
				reply += IntToStr(i);
				reply += "\n";
			}
		}
	}
	//save
	else if(message == "-save" && player.IsImm())
	{
		//serverPtr->BackUp("rooms.mrf");
		serverPtr->SaveRooms("rooms.mrf");
		serverPtr->SaveSkills("skills.msf");
		reply += "\nRooms and skills saved.\n";
	}
	//get skills
	else if(message == "-slist" && player.IsImm())
	{
		reply += "\nSkills:\n";
		reply += "-----------------\n";
		for(int i = 0; i < serverPtr->GetSkills()->size(); i++)
		{
			reply += "+";
			reply += (*serverPtr->GetSkills())[i].GetName();
			reply += " ID:";
			reply += IntToStr((*serverPtr->GetSkills())[i].GetID());
			reply += "\n";
		}
	}
	//create skill
	else if(message == "-s" && player.IsImm())
	{
		Skill newskill = Skill("newSkill", false, 0, Skill::NextSkillID(), serverPtr->GetLua());
		serverPtr->GetSkills()->push_back(newskill);
		reply += "\nNew skill made. ID:";
		reply += IntToStr(newskill.GetID());
		reply += "\n";
	}
	//rename skill
	else if(message.substr(0, 7) == "-sname " && player.IsImm())
	{
		int id = StrToInt(message.substr(7, 1));
		(*serverPtr->GetSkills())[id].SetName(message.substr(9, message.length()-9));
		reply += "\nSkill #";
		reply += IntToStr(id);
		reply += " renamed to ";
		reply += message.substr(9, message.length()-9);
		reply += ".\n";
	}
	//skill toggle passive
	else if(message.substr(0, 7) == "-spass " && player.IsImm())
	{
		int id = StrToInt(message.substr(7, 1));
		(*serverPtr->GetSkills())[id].SetPassive(!(*serverPtr->GetSkills())[id].GetPassive());
		if((*serverPtr->GetSkills())[id].GetPassive())
		{
			reply += "\nSkill #";
			reply += IntToStr(id);
			reply += " is now passive.\n";
		}
		else
		{
			reply += "\nSkill #";
			reply += IntToStr(id);
			reply += " is now active.\n";
		}
	}
	//skill cost
	else if(message.substr(0, 7) == "-scost " && player.IsImm())
	{
		int id = StrToInt(message.substr(7, 1));
		(*serverPtr->GetSkills())[id].SetCost(StrToInt(message.substr(9, message.length()-9)));
		reply += "\nSkill #";
		reply += IntToStr(id);
		reply += " now costs ";
		reply += message.substr(9, message.length()-9);
		reply += " sp to use.\n";
	}
	//get skill lua file
	else if(message.substr(0, 9) == "-sgetlua " && player.IsImm())
	{
		int id = StrToInt(message.substr(9, 1));
		reply += "\n";
		reply += (*serverPtr->GetSkills())[id].GetName();
		reply += ".lua:\n";
		(*serverPtr->GetSkills())[id].GetLuaFile(reply);
	}
	//set skill lua file
	else if(message.substr(0, 9) == "-ssetlua " && player.IsImm())
	{
		int id = StrToInt(message.substr(9, 1));
		reply += "\n";
		reply += (*serverPtr->GetSkills())[id].GetName();
		reply += ".lua set.\n";
		(*serverPtr->GetSkills())[id].SetLuaFile(message.substr(10, message.length()-1));
	}
	//kick
	else if(message.substr(0, 6) == "-kick " && player.IsImm())
	{
		reply += "\n is now kicked from the server.\n";
	}
	reply += player.GetPrompt();
	return reply;
}