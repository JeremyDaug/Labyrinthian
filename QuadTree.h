#ifndef QUADTREE_H
#define QUADTREE_H

#include "Node.h"

class QuadTree
{
private:
	// effectively constant. It always stradles the four major quadrants.
	// Should always have a pos of -1/2, -1/2. 
	// as this will put the axes in the tree as well. (+/-, 0 and 0, +/-) 
	Node* head;
	Node* FocusedCell;
	Node* FocusedRoom;
	Node* FocusedTile;
	
	int height;

public:
	void makeTileSpace(Point pos);
	int* findTileData
};

#endif // QUADTREE_H
