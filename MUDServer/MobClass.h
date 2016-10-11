#pragma once

#include <string>
#include "Stats.h"

class MobClass
{
private:
	//class info
	std::string classname;

	//inherited attributes
	std::string mobname;
	std::string mobdesc;
	int hp, hpMax; //hit points
	int sp, spMax; //spell points
	int mv, mvMax; //movement points
	Stats stats; //stats
public:
	MobClass(void);
	~MobClass(void);
};
