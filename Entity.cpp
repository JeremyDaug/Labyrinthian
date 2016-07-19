#include "Entity.h"
#include <math.h>

Entity::Entity()
{
	// give an available ID for the entity.
	
	// fill out skill list.
}

Entity::~Entity()
{
}

int Entity::maxHP() 
{
	return (int)std::floor(End() * (Level() * scale.hp));
}

int Entity::maxMP()
{
	return (int)std::floor(Wil() * (Level() * scale.mp));
}

int Entity::maxSP()
{
	return (int)std::floor(End()*scale.sp);
}

int Entity::maxWP()
{
	return (int)std::floor(Wil()*scale.wp);
}

int Entity::Level()
{
	int ret = 0;
	for (unsigned int i = 0; i < levels.size(); ++i)
	{
		ret += levels[i];
	}
	return ret;
}
