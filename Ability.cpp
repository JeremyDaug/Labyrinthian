#include "Ability.h"



Ability::Ability()
{
}


Ability::~Ability()
{
}

std::vector<std::string>* Ability::getExtra()
{
	std::vector<std::string>* ret = &std::vector<std::string>(extra);
	return ret;
}

void Ability::setCent(float x)
{
	if (x > 1 || x < -1 || x == 0)
		x = std::fmod(x, 1);
	cent = x;
	return;
}
