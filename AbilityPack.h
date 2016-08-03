#ifndef __ABILITYPACK_H
#define __ABILITYPACK_H

#include "Ability.h"
#include <vector>
#include <map>
#include <string>

class AbilityPack
{
private:
	// useful data
	std::string name;
	std::string desc;

	// the storage list
	std::vector<Ability*> abilities;

	// requriements
	// Everything it needs is under itself.
	// if a requirement is an ability then the key for that
	// ability is it's name. It will hold the string "true"
	std::map<std::string, std::string> reqs;

public:
	AbilityPack();
	~AbilityPack();

	// getters
	std::string getName() { return name; }
	std::string getDesc() { return desc; }
	Ability* getAbility(int index);
	std::string HasRequirement(std::string req);
	std::string RequiresFrom(std::string key);
	bool meetsRequirements(std::string key, std::string has) { return reqs[key] == has; }
};

#endif // __ABILITYPACK_H