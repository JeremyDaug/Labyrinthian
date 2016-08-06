#ifndef __ABILITY_H
#define __ABILITY_H

#include <string>
#include <vector>
#include <math.h>

// include constants
#include "Constants.h"

class Ability
{
private:
	//search id
	int id;

	// info
	std::string shortdesc;
	std::string longdesc;

	// modifiers
	std::string to;
	int flat;
	float cent;
	short flags;

	// cost of the ability
	int cost;

	// Extra storage for miscellanious info (usually connected to flags)
	std::vector<std::string> extra;

	// very special stuff here.
	std::string allowance;

	// setters The only class that should alter this is the AbilityPack Class.
	void setShortDesc(std::string x) { shortdesc = x; }
	void setLongDesc(std::string x) { longdesc = x; }
	void setFlat(int x) { flat = x; }
	void setCent(float x);
	void setFlags(short f) { flags = f; }
	void addExtra(std::string x) { extra.push_back(x); }
	void setAllowance(std::string x) { allowance = x; }

public:
	Ability();
	~Ability();

	// getters
	int getId() { return id; }
	std::string getShortDesc() { return shortdesc; }
	std::string getLongDesc() { return longdesc; }
	std::string getTo() { return to; }
	int getFlat() { return flat; }
	float getCent() { return cent; }
	short getFlags() { return flags; }
	int getCost() { return cost; }
	std::vector<std::string>* getExtra();
	std::string getAllowance() { return allowance; }

	friend class AbilityPack;
};

#endif