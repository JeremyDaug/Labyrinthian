#ifndef _CELL_H
#define _CELL_H

struct Point
{
	long x;
	long y;
	Point() : x(0), y(0) {}
	Point(long nx, long ny) : x(nx), y(ny) {}
	Point(const Point& pos) : x(pos.x), y(pos.y) {}

	bool operator==(const Point& rhs) { return (x == rhs.x && y == rhs.y); } //  Tested
	bool operator!=(const Point& rhs) { return !this->operator==(rhs); } // Tested
	bool operator<(const Point& rhs) const // Tested
	{
		if (x == rhs.x)
		{
			return y < rhs.y;
		}
		return x < rhs.x;
	}
};

enum ConnStat : short
{
	closed = 0,
	open = 1,
	locked = 2,
	unlocked = 3
};

enum DirBitmask : short
{
	Nmask = 64 + 128,
	Emask = 16 + 32,
	Smask = 4 + 8,
	Wmask = 1 + 2
};

enum Direction : char
{
	north = 'N',
	east = 'E',
	south = 'S',
	west = 'W'
};

class Cell
{
	/*
	Cells are to never be used in any other class directly. You should not pass, nor return a cell
	to or from another class. You can screw it up and this class has few safety measures built directly
	into it. If needed they will be added, but I'd rather not bother.
	*/

	// what is in the cell.
	int data[64][64][5];
	// how the rooms connect to each other.
	short connectivity[8][8];
	/*
	the data in the short has 4 states, 0-closed, 1-Open, 2-locked, and 3-unlocked.
	A short is used as a bit array for ease of use. If you need to access it, use the connMask function or
	getDirectionalConnectivity function.
	first - N, second - E, third - S, fourth - W.
	connectivity that is completely closed (ie. == 0), it is considered nonexistent.

	The only allowed mismatch in connection is between a room with a locked connection and a room that
	doesn't exist (ie has a connectivity of 0)
	*/

	// Utility mask for connectivity
	// only Takes N, E, S, and W
	ConnStat connMask(short connectivity, Direction dir);

	int reverseMask(Direction dir, ConnStat type);

	// a more specific wall updater function to be used on specific rooms.
	void updateWallsToRoom(Point room);

	// update the walls to match their connectivity
	// checks all rooms for this
	void updateWallsToConnectivity();

public:
	// Check all the rooms for proper connection to this room.
	bool checkRoomConnections(Point pos); // Tested

	bool ConnectivityCheck(); // Tested
							  // Default Constructor
	Cell(); // Tested
			// setters.
	void setData(Point pos, int ndata, int level) { data[pos.x][pos.y][level] = ndata; } // tested
	void setConnectivity(Point pos, Direction Dir, ConnStat type); // Tested
	// getters for tile data.
	int* getData(int x, int y) { return data[x][y]; } // tested
	int* getData(Point pos) { return data[pos.x][pos.y]; } // tested
														   // getter for all connectivity of a room.
	short getConnectivity(int x, int y) { return connectivity[x][y]; } // tested
	short getConnectivity(const Point& pos) { return connectivity[pos.x][pos.y]; } // tested																	   // getter of specific connectivity

	// returns the 0-3, for the state of the room, not the full char.
	ConnStat getDirectionalConnectivity(Point& pos, Direction Dir); // tested
	ConnStat getDirectionalConnectivity(int x, int y, Direction Dir) { return getDirectionalConnectivity(Point(x, y), Dir); } // tested

	bool roomExists(Point pos) { return connectivity[pos.x][pos.y] != 0; };
};


#endif // _CELL_H