#pragma once

#include <string>
#include <iostream>
#include <fstream>

#include "LuaFunctions.h"

struct skillHeader
{
	int skills;
};

struct skillSave
{
	char name[100]; //name of skill
	bool passive;
	int spcost;
	int id;
};

class Skill
{
private:
	static int skillCount;
	std::string name; //name of skill - will have lua file name.lua
//lua file will have these functions:
	//OnStart(player, target) called when skill is cast
	//OnCast(player, target) called when timer is done
	//OnEnd(player, target) called when skill duration ends
	bool passive;
	int spcost;
	int id; //skill id
	lua_State* luaPtr;
public:
	static int NextSkillID();
	Skill();
	Skill(const std::string& name, bool passive, int spcost, int id, lua_State* luaPtr);
	~Skill();
	void SetSkill(const std::string& name, bool passive, int spcost, int id, lua_State* luaPtr);

	void SetLuaFile(const std::string& text); //sets the lua file for the skill
	void GetLuaFile(std::string& text); //gets the lua file for the skill and stores it in text

	void OnStart(int player, int target);
	void OnCast(int player, int target);
	void OnEnd(int player, int target);

	skillSave ToSkillSave();
	void FromSkillSave(const skillSave& save, lua_State* luaPtr);

	void SetName(const std::string& name);
	void SetPassive(bool passive);
	void SetCost(int spcost);
	std::string GetName();
	bool GetPassive();
	int GetID();
};
