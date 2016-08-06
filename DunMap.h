#ifndef DUNMAP_H
#define DUNMAP_H

#include<map>
#include<vector>
#include<queue>

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

	// A hard setter for creating room connections, adds the connection regardless of the rooms, even to
	// the point of breaking connectivity. Use lightly.
	void SetRoomConnectivity(Point& cell, Point& room, Direction dir, ConnStat connType);

	// Another Hard Setter for Creating room connection, but does so mutually, ensuring that connectivity
	// is retained. Ignores preexisting connections.
	void SetMutualRoomConnectivity(Point& cell, Point& room, Direction dir, ConnStat connType);

	// Connectivity getter for an arbitrary cell, room, and direction.
	ConnStat getConnectionFor(Point& cell, Point& room, Direction dir) { return BigMap[cell]->getDirectionalConnectivity(room, dir); }

	// a utility function primarily for testing if a room is validly connected to another.
	std::vector<Point> findPath(Point& start, Point& startRoom, Point& end, Point& endRoom);

	// A more specific function to find if it returns to the town.
	Point TownCell = Point(0, 0);
	Point TownRoom = Point(4, 4);
	std::vector<Point> findPathToTown(Point& start, Point& startRoom) { return findPath(start, startRoom, TownCell, TownRoom); }

	// change the tiles along the walls to match their connectivity, assume connections are true, 
	// run connectivity check before doing this.
	void updateWalls();

	// another wall updater, but for specific rooms.
	void updateRoomsWalls(Point& cell, Point& room);

	// TODO add some functions to get undir, adj room, and adj cell.

	// Check for interconnectivity between cells (ensure they match up).
	bool InterCellConnectivityCheck(Point QuestionCell);

public:
	// Functions

	// Point Data Setters
	void setData(Point& pos, int ndata, int level) { curr->setData(pos, ndata, level); }
	void setDataInCell(Point& CellPos, Point& ptPos, int ndata, int level);

	// Find if a Cell exists already
	bool CellExists(Point& pos);
	bool CellExists(long x, long y) { return CellExists(Point(x, y)); }

	// getters for tile data
	int* getTileDataInCell(Point& cell, Point& room, Point& tile) { return BigMap[cell]->getData(Point(room.x * 8 + tile.y, room.y * 8 + tile.y)); }
	// getter for a tile in the curr cell.
	int* getTileData(Point& room, Point& tile) { return getTileDataInCell(currPos, room, tile); }

	// Constructor
	DunMap();
	// Destructor
	~DunMap();

	// Create room connection even between cells. This will only go through if the existing connection types agree with it.
	// either locked<->DNE to unlocked<->unlocked or like<->like, there are additional rules applied by the game. See overview for details.
	// We take the assumption that the cell exists and the room exists (is not DNE or 0).
	bool CreateRoomConnection(Point& room, Direction dir, ConnStat connType) { return CreateRoomConnectionInCell(currPos, room, dir, connType); }
	bool CreateRoomConnectionInCell(Point& cell, Point& room, Direction dir, ConnStat connType);

	bool toggleRoomLock(Point& cell, Point& room, Direction dir);
	bool toggleRoomLockInCurr(Point& room, Direction dir) { return toggleRoomLock(currPos, room, dir); }

	// Create Cell
	// if the cell given doesn't exist it makes it. else it does nothing.
	// Param: Pos, the point of the cell
	void CreateCell(Point& room);
	void CreateCell(long x, long y) { return CreateCell(Point(x, y)); }

	// Create Blank room. This is for testing purposes. It is a room with one type of each connection type.
	// We this will be used to test creation based on connection logic.
	// Returns true if the room was created, false if the room already exists.
	void CreateBlankRoom(Point& pos); // no randomizer, make blank room. 
	void CreateDeadRoom(Point& pos); // Even less testing here, just create a room to test gfx.

	bool CreateRoom(Point& room);
	bool CreateRoom(long x, long y) { return CreateRoom(Point(x, y)); }

	// Loader Function (placeholder)
	// void Load(stuff);

	// tester functions
	bool ConnectivityConsistencyCheck(); // tested
	// Pathing check.

	// Return the data of a cell
	void getCellData(Point cell, unsigned char* tiles);
	void getCurrCellData(unsigned char* tiles) { getCellData(currPos, tiles); }

	void getRoomData(Point cell, Point room, unsigned char* tiles);
};

#endif // !DUNMAP_H