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

	// Getters that get tile data from the cells.
	// The array is read left to right, top to bottom, like a book.
	// This is not static, this can be changed if needed.
	// I suggest using point, but there is an option to use numbers
	// but it just delegates to the Point version.
	// as a note it returns a cell not a room, so it is 64x64
	// tiles should be null so that it can be filled.
	void getCellData(long x, long y, unsigned char* tiles) { getCellData(Point(x, y), tiles); }
	void getCellData(Point cell, unsigned char* tiles);

	// getters for a specific room. This is probably what sean meant.
	// it takes a Point for both cell and room, room is entirely between 0-7 on both x and y.
	void getRoomData(Point cell, Point room, unsigned char* tiles);
	void getRoomData(long cellx, long celly, long roomx, long roomy, unsigned char* tiles) { getRoomData(Point(cellx, celly), Point(roomx, roomy), tiles); }
};

#endif // __GM_H