#include "GM.h"



GM::GM()
{
}


GM::~GM()
{
}

void GM::getCellData(Point cell, unsigned char * tiles)
{
	Map.getCellData(cell, tiles);
}

void GM::getRoomData(Point cell, Point room, unsigned char* tiles)
{
	Map.getRoomData(cell, room, tiles);
}
