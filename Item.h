#ifndef __ITEM_H
#define __ITEM_H

#include <string>
#include <vector>
#include "AbilityPack.h"

class Item
{
	// general info
	std::string name;
	std::string desc;

	// bonuses given by the item
	std::vector<AbilityPack> bonuses;

	// flags for stuff.
	unsigned long slotflags;
	unsigned long extraflags;

	// extra space for stuff as needed.
	std::vector<std::string> extra;

public:
	Item();
	~Item();
};

#endif // __ITEM_H