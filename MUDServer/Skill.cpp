#include "Skill.h"

int Skill::skillCount = 0;

int Skill::NextSkillID()
{
	return Skill::skillCount++;
}

Skill::Skill(void)
{
}

Skill::Skill(const std::string& name, bool passive, int spcost, int id, lua_State* luaPtr)
{
	this->SetSkill(name, passive, spcost, id, luaPtr);
}

Skill::~Skill(void)
{
}

void Skill::SetSkill(const std::string &name, bool passive, int spcost, int id, lua_State* luaPtr)
{
	this->name = name;
	this->passive = passive;
	this->spcost = spcost;
	this->id = id;
	this->luaPtr = luaPtr;
}

void Skill::SetLuaFile(const std::string& text)
{
	//filename for lua file
	std::string filename = "skills/";
	filename += this->name;
	filename += ".lua";

	std::ofstream ofs(filename.c_str(), std::ios::trunc | std::ios::out);
	ofs << text;
	ofs.close();
	luaL_dofile(luaPtr, filename.c_str());
}

void Skill::GetLuaFile(std::string& text)
{
	//filename for lua file
	std::string filename = "skills/";
	filename += this->name;
	filename += ".lua";

	std::ifstream ifs(filename.c_str(), std::ios::in);
	std::string buffer;
	while(!ifs.fail())
	{
		std::getline(ifs, buffer);
		text += buffer;
		text += "\n";
	}
	ifs.close();
}

void Skill::SetName(const std::string &name)
{
	//old filename
	std::string oldfilename = "skills/";
	oldfilename += this->name;
	oldfilename += ".lua";
	
	//filename for lua file
	std::string filename = "skills/";
	filename += name;
	filename += ".lua";

	

	//rename skill
	this->name = name;

	//rename old file
	if(rename(oldfilename.c_str(), filename.c_str()) != 0)
	{
		//create lua file named after skill
		std::ofstream ofs(filename.c_str(), std::ios::out);
		ofs << "--Skill: " << name << std::endl
			<< std::endl
			<< "function OnStart(player, target)" << std::endl
			<< "	StartTimer(player, target, " << id << ", 1000)" << std::endl
			<< "end" << std::endl
			<< std::endl
			<< "function OnCast(player, target)" << std::endl
			<< "	StartDuration(player, target, " << id << ", 1000)" << std::endl
			<< "end" << std::endl
			<< std::endl
			<< "function OnEnd(player, target)" << std::endl
			<< "end" << std::endl;
		ofs.close();
	}
}

void Skill::SetPassive(bool passive)
{
	this->passive = passive;
}

void Skill::SetCost(int spcost)
{
	this->spcost = spcost;
}

std::string Skill::GetName()
{
	return this->name;
}
bool Skill::GetPassive()
{
	return this->passive;
}

int Skill::GetID()
{
	return this->id;
}

/*int StartTimer(Lua_state* L)
{
	char* skillname = firstarg();
	int time = secondarg();
	playerPtr->BeginTimer(skillname, time);
}

int StartDuration(Lua_state* L)
{
	char* skillname = firstarg();
	int time = secondarg();
	playerPtr->BeginTimer(skillname, time);
}*/

void Skill::OnStart(int player, int target)
{
	//std::cout << "Skill " << this->name << " started from player #" << player << " on player #" << target << std::endl;
	std::string filename = "skills/";
	filename += name;
	filename += ".lua";

	luaL_dofile(luaPtr, filename.c_str());
	lua_getglobal(luaPtr, "OnStart");
	lua_pushnumber(luaPtr, player);
	lua_pushnumber(luaPtr, target);
	lua_call(luaPtr, 2, 0);
	//lua_pop(luaPtr, 1);
}

void Skill::OnCast(int player, int target)
{
	//std::cout << "Skill " << this->name << " casted from player #" << player << " on player #" << target << std::endl;
	std::string filename = "skills/";
	filename += name;
	filename += ".lua";

	luaL_dofile(luaPtr, filename.c_str());
	lua_getglobal(luaPtr, "OnCast");
	lua_pushnumber(luaPtr, player);
	lua_pushnumber(luaPtr, target);
	lua_call(luaPtr, 2, 0);
	//lua_pop(luaPtr, 1);
}
void Skill::OnEnd(int player, int target)
{
	//std::cout << "Skill " << this->name << " ended from player #" << player << " on player #" << target << std::endl;
	std::string filename = "skills/";
	filename += name;
	filename += ".lua";

	luaL_dofile(luaPtr, filename.c_str());
	lua_getglobal(luaPtr, "OnEnd");
	lua_pushnumber(luaPtr, player);
	lua_pushnumber(luaPtr, target);
	lua_call(luaPtr, 2, 0);
	//lua_pop(luaPtr, 1);
}

skillSave Skill::ToSkillSave()
{
	skillSave ret;
	sprintf(ret.name, "%s\0", this->name.c_str());
	ret.passive = this->passive;
	ret.spcost = this->spcost;
	ret.id = this->id;
	return ret;
}

void Skill::FromSkillSave(const skillSave& save, lua_State* luaPtr)
{
	this->name = save.name;
	this->passive = save.passive;
	this->spcost = save.spcost;
	this->id = save.id;
	this->luaPtr = luaPtr;	
}