#include "AbilityPack.h"



AbilityPack::AbilityPack()
{
}


AbilityPack::~AbilityPack()
{
}

Ability* AbilityPack::getAbility(int index)
{
	if (abilities.size() <= index)
		return nullptr;

	return abilities[index];
}

std::string AbilityPack::HasRequirement(std::string req)
{
	if (reqs.find(req) == reqs.end())
		return NONE;
	return reqs[req];
}

std::string AbilityPack::RequiresFrom(std::string key)
{
	if (HasRequirement(key) == NONE)
		return NONE;

	return reqs[key];
}
