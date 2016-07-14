#ifndef DUNMAP_H
#define DUNMAP_H
// TODO Find why the vector isn't liked.

#include<map>
#include<vector>

struct Point
{
	long x;
	long y;
	Point() : x(0), y(0) {}
	Point(long nx, long ny) : x(nx), y(ny) {}

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

enum ConnStat
{
	closed = 0,
	open = 1,
	locked = 2,
	unlocked = 3
};

enum Direction
{
	north = 'N',
	east = 'E',
	south = 'S',
	west = 'W'
};

class Cell
{
	// what is in the room.
	int data[64][64][5];
	// how the rooms connect to each other.
	char connectivity[8][8];
	/*
	the data in the char has 4 states, 0-closed, 1-Open, 2-locked, and 3-unlocked.
	A char has bits and each pair creates the end result.
	first - N, second - E, third - S, fourth - W.
	connectivity that is completely closed (ie. == 0) or locked, it is considered nonexistent. 

	if a pair of rooms have a mismatch where one is == 0 and the other is locked, the test does not fail.
	This case indicates that the room the lock points to does not exist.
	*/

	// Utility mask for connectivity
	// only Takes N, E, S, and W
	ConnStat connMask(char connectivity, char dir);

	// Room connection Valid checker
	// Takes a room and the adjacent room you want to check on.
	// Returns true if adjacent room is outside of the cell as intercell connections are checked in DunMap.
	bool roomConnValid(Point room, char dir);

	char reverseMask(char dir, ConnStat type);

public:
	bool ConnectivityCheck(); // Tested
	// Default Constructor
	Cell();
	// setters.
	void setData(Point pos, int ndata, int level) { data[pos.x][pos.y][level] = ndata; } // tested
	void setConnectivity(Point pos, char Dir, ConnStat type);
	// getters for tile data.
	int* getData(int x, int y) { return data[x][y]; } // tested
	int* getData(Point pos) { return data[pos.x][pos.y]; } // tested
	// getter for all connectivity of a room.
	char getConnectivity(int x, int y) { return connectivity[x][y]; }
	char getConnectivity(Point pos) { return connectivity[pos.x][pos.y]; }

	// getter of specific connectivity
	// returns the 0-3, for the state of the room, not the full char.
	ConnStat getDirectionalConnectivity(Point pos, char Dir);
	ConnStat getDirectionalConnectivity(int x, int y, char Dir) { return getDirectionalConnectivity(Point(x, y), Dir); }
};

class DunMap
{
    // Current Cell
	Cell* curr;

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

    // Functions
	// Check for interconnectivity between cells (ensure they match up).
	bool InterCellConnectivityCheck(Point QuestionCell);

public:
	// Point Data Setters
	void setData(Point pos, int ndata, int level) { curr->setData(pos, ndata, level); }
	void setDataInCell(Point CellPos, Point ptPos, int ndata, int level);

	// Find if a Cell exists already
	bool CellExists(Point pos);
	bool CellExists(long x, long y) { return CellExists(Point(x, y)); }

	// Let's start building.
	// Constructor
	DunMap();
	// Destructor
	~DunMap();

	// Create Cell
	// Returns True if it was created, false if the cell is already taken.
	// Param: Pos, the point of the cell
	bool CreateCell(Point pos);
	bool CreateCell(long x, long y) { return CreateCell(Point(x, y)); }

	// Create Room
	// Returns true if the room was created, false if the room already exists.
	bool CreateRoom(Point pos, int(*randomizer)());
	bool CreateRoom(long x, long y, int(*randomizer)()) { return CreateRoom(Point(x, y), randomizer); }

	// Loader Function (placeholder)
	// void Load(stuff);

	// tester functions
	bool ConnectivityConsistencyCheck();
	// Pathing check.
};

#endif // !DUNMAP_H