#ifndef DUNMAP_H
#define DUNMAP_H
// TODO Find why the vector isn't liked.

#include<map>
#include<vector>

#include "Cell.h"


class DunMap
{
    // Current Cell
	Cell* curr;
	Point currPos;

    // Surrounding Cells
	/* The layout of near cells is.
	7 0 1
	6 - 2
	5 4 3
	*/
	Cell* near[8];

	// sanity checker for whether a cell exists or not. probably need a better way.
	std::vector<Point> existingCells;

    // The Map itself.
	std::map<Point, Cell*> BigMap;

	// A hard setter for creating room connections, adds the connection regardless of the rooms, but ensures
	// the consistency of the map remains. Will even create a 'dead room' that is completely detached.
	void SetRoomConnections(Point& cell, Point& room, Direction dir, ConnStat connType);

public:
	// Functions
	// Check for interconnectivity between cells (ensure they match up).
	bool InterCellConnectivityCheck(Point QuestionCell);

	// Point Data Setters
	void setData(Point& pos, int ndata, int level) { curr->setData(pos, ndata, level); }
	void setDataInCell(Point& CellPos, Point& ptPos, int ndata, int level);

	// Find if a Cell exists already
	bool CellExists(Point& pos);
	bool CellExists(long x, long y) { return CellExists(Point(x, y)); }

	// Let's start building.
	// Constructor
	DunMap();
	// Destructor
	~DunMap();

	// Create room connection even between cells. This will only go through if the rooms don't exist.
	// returns false if the room already exists.
	bool CreateRoomConnection(Point& room, Direction dir, ConnStat connType) { return CreateRoomConnectionInCell(currPos, room, dir, connType); }
	bool CreateRoomConnectionInCell(Point& cell, Point& room, Direction dir, ConnStat connType);

	// Create Cell
	// Returns True if it was created, false if the cell is already taken.
	// Param: Pos, the point of the cell
	bool CreateCell(Point& pos);
	bool CreateCell(long x, long y) { return CreateCell(Point(x, y)); }

	// Create Blank room. This is for testing purposes. It is a room with one type of each connection type.
	// We this will be used to test creation based on connection logic.
	// Returns true if the room was created, false if the room already exists.
	bool CreateBlankRoom(Point& pos); // no randomizer, make blank room.
	bool CreateBlankRoomInCell(Point& cell, Point room);

	bool CreateRoom(Point& pos, int(*randomizer)());
	bool CreateRoom(long x, long y, int(*randomizer)()) { return CreateRoom(Point(x, y), randomizer); }

	// Loader Function (placeholder)
	// void Load(stuff);

	// tester functions
	bool ConnectivityConsistencyCheck(); // tested
	// Pathing check.
};

#endif // !DUNMAP_H