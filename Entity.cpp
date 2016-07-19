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
	return std::floor(End() * (Level() * scale.hp));
}

int Entity::maxMP()
{
	return std::floor(Wil() * (Level() * scale.mp));
}

int Entity::maxSP()
{
	return std::floor(End()*scale.sp);
}

int Entity::maxWP()
{
	return std::floor(Wil()*scale.wp);
}

int Entity::Level()
{
	int ret = 0;
	for (int i = 0; i < levels.size; ++i)
	{
		ret += levels[i];
	}
	return ret;
}
