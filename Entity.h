#ifndef _ENTITY_H
#define _ENTITY_H

#include <string>
#include <vector>
#include <map>
#include "AbilityPack.h"

// constants
#include "Constants.h"

// A holder for our standard values. May float off to a config
// file, but unlikely as this should not be changeable by the player ordinarily.
const struct Scales
{
	float hp = 1.2;
	float mp = 1.2;
	float sp = 1.5;
	float wp = 1.5;
} scale;

class Entity
{
private:
	// a unique, semi-random id to denote a specific entity.
	int ID;

	// The name of the entity not required to be unique.
	std::string Name;

	// Stat[0][x] are physical.
	// Stat[1][x] are Mental.
	// Stat[0][0] is Strength.
	// Stat[0][1] is Dexterity.
	// Stat[0][2] is Endurance.
	// Stat[1][0] is Intelligence.
	// Stat[1][1] is Intuition.
	// Stat[1][2] is WillPower.
	unsigned int Stats[2][3];

	// the Condition Bars. HP, SP, MP, and WP
	// Health Points (HP) is the ammount of damage the creature can take.
	// Stamina Points (SP) is the measure of it's physical energy.
	// Mental Points (MP) is like HP but for the Mind.
	// Will Points (WP) is like SP for the mind.
	unsigned int Condition[2][2];

	// a bitmap for the types that the entity counts as.
	// some are exclusive to each other, but most are not.
	// Depending on it's type the properties of the entity can change.
	// Ex. An undead does not have WP or SP.
	int types;

	// Specializations and level in them.
	// std::vector<Spec> specs;
	std::vector<int> levels;

	// Skill List
	std::map<std::string, int> skills;

	// Unspec'd Abilities, both gained and earned.
	std::vector<AbilityPack> abilities;

	// items
	// weapons [equiped/unequiped]
	// armor [equiped]
	// equimpent [2 rings, 2 bands, boots, gloves, capes, eyes, belt, pack, neck, helmet]
	// inventory [weapons, armor, equipment, consumables]

	// permanent (slotless) enhancements many are exclusive to each other, and that is the central limit.
	// std::vector<enhancements> boosts;

public:
	Entity();
	~Entity();

	// Max functions for HP,SP,MP, and WP
	unsigned int maxHP();
	unsigned int maxMP();
	unsigned int maxSP();
	unsigned int maxWP();

	// Stat Accessors
	unsigned int Str() {	return Stats[0][0]; }
	unsigned int Dex() { return Stats[0][1]; }
	unsigned int End() { return Stats[0][2]; }
	unsigned int Int() { return Stats[1][0]; }
	unsigned int Itu() { return Stats[1][1]; }
	unsigned int Wil() { return Stats[1][2]; }

	// Current Total Level
	unsigned int Level();
};

#endif // _ENTITY_H