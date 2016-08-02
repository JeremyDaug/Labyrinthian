#ifndef __ABILITY_H
#define __ABILITY_H

#include <string>
#include <vector>
#include <math.h>

// defines
#define STR "strength"
#define DEX "dexterity"
#define CON "constitution"
#define INT "intelligence"
#define ITU "intuition"
#define WIL "willpower"

#define HP "healthPoints"
#define SP "staminaPoints"
#define MP "mentalPoints"
#define WP "willPoints"

enum mods : unsigned short
{
	stackable = 1,
	percent = 2,
	flat = 4,
	allowance = 8,
	DoT = 16, // damage over time
	AoE = 32, // Area of Effect
	attack = 64, // whether it is usable on others or only yourself.
	magic = 128,
	tech = 256,
	physical = 512,

};

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

	// Extra storage for miscellanious info (usually connected to flags)
	std::vector<std::string> extra;

	// very special stuff here.
	std::string allowance;

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
	std::vector<std::string>* getExtra();
	std::string getAllowance() { return allowance; }

	// setters
	void setShortDesc(std::string x) { shortdesc = x; }
	void setLongDesc(std::string x) { longdesc = x; }
	void setFlat(int x) { flat = x; }
	void setCent(float x);
	void setFlags(short f) { flags = f; }
	void addExtra(std::string x) { extra.push_back(x); }
	void setAllowance(std::string x) { allowance = x; }

};

#endif