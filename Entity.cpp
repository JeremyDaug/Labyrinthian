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

unsigned int Entity::maxHP() 
{
	return (unsigned int)std::floor(End() * (Level() * scale.hp));
}

unsigned int Entity::maxMP()
{
	return (int)std::floor(Wil() * (Level() * scale.mp));
}

unsigned int Entity::maxSP()
{
	return (int)std::floor(End()*scale.sp);
}

unsigned int Entity::maxWP()
{
	return (int)std::floor(Wil()*scale.wp);
}

unsigned int Entity::Level()
{
	int ret = 0;
	for (unsigned int i = 0; i < levels.size(); ++i)
	{
		ret += levels[i];
	}
	return ret;
}
