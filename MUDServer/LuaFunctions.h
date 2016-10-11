#pragma once
//All functions that will be accessable by Lua will be declared here
//Most of these will be defined in server.cpp or skill.cpp
extern "C"
{
	#include "lua.h"
	#include "lualib.h"
	#include "lauxlib.h"
}

//Timers
int StartTimer(lua_State* L); //playerid, targetid, skillid, time
int StartDuration(lua_State* L); //playerid, targetid, skillid, time

//Information about mobs
int GetRoomID(lua_State* L); //playerid
int GetName(lua_State* L); //playerid
int IsImm(lua_State* L); //playerid

//Sending text
int SendToRoom(lua_State* L); //roomid, text
int SendTo(lua_State* L);  //playerid, text

//Modifying mobs
int SetHealth(lua_State* L); //playerid, value
int SetRoundTime(lua_State* L); //playerid, time
int GetHealth(lua_State* L); //playerid
int GetRoundTime(lua_State* L); //playerid

int SetCanMove(lua_State* L); //playerid, bool
int SetCanAttack(lua_State* L); //playerid, bool
int SetCanSee(lua_State* L); //playerid, bool
int SetCanTalk(lua_State* L); //playerid, bool
int CanMove(lua_State* L); //playerid
int CanAttack(lua_State* L); //playerid
int CanSee(lua_State* L); //playerid
int CanTalk(lua_State* L); //playerid

