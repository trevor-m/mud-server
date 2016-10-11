#pragma once

enum Stat
{
	Str,
	Int,
	Wil,
	Dex,
	Con,
	STAT_COUNT
};

class Stats
{
private:
	int stats[STAT_COUNT];
public:
	Stats();
	void Randomize(); //a roll of the dice
	int GetStat(Stat stat); //returns value for specified stat
};
