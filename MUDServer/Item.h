#pragma once

#include <string>
#include "Stats.h"

class Item
{
private:
	static int itemCount;
	std::string name; //name of the item 'sword'
	std::string desc; //brief description 'A sword rests here.'
	std::string exam; //information about the item 'A sharp broadsword with a fine tip. +2 str and -1 dex'
	Stats bonuses;
public:
	static int NextItemID();
	Item(void);
	Item(int id, const std::string& name, const std::string& desc, const std::string& exam, Stats bonuses);
	~Item(void);

	const std::string& GetName();
	const std::string& GetDescription();
};
