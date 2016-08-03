#ifndef __GM_H
#define __GM_H

#include <vector>
#include "AbilityPack.h"
#include "DunMap.h"
#include "Entity.h"

/*
A class to hold everything and organize it nicely for mulitple use, this class is also the
'director' of the game. It will have AI with the goal of challenging the player fairly for their
location in the game. This should also be the main entry point for the Graphic's engine to be
attached. This is far from finalized and will probably change a thousand times.
*/

class GM
{
private:
	// Loaded Data

	// Ability Packs we do not store individual abilities.
	std::vector<AbilityPack> Packs;

	// characters
	std::vector<Entity> entities;

	// THE MAP!!!!!!
	DunMap Map;

	// Items

public:
	GM();
	~GM();
};

#endif // __GM_H