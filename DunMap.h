#ifndef DUNMAP_H
#define DUNMAP_H

#include<map>
#include<vector>
using namespace std;

struct Point
{
	long x;
	long y;
	Point() : x(0), y(0) {}
	Point(long nx, long ny) :  x(nx), y(ny) {}

	inline bool operator==(const Point& rhs) { return (x == rhs.x && y == rhs.y); }
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

	bool ConnectivityCheck();

public:
	// Default Constructor
	Cell();
	// setters.
	void setData(Point pos, int ndata, int level) { data[pos.x][pos.y][level] = ndata; }
	// getters for tile data.
	int* getData(int x, int y) { return data[x][y]; }
	int* getData(Point pos) { return data[pos.x][pos.y]; }
	// getter for all connectivity of a room.
	char getConnectivity(int x, int y) { return connectivity[x][y]; }
	char getConnectivity(Point pos) { return connectivity[pos.x][pos.y]; }

	// Utility mask for connectivity
	// only Takes N, E, S, and W
	char connMask(char connectivity, char dir);

	// getter of specific connectivity
	// returns the 0-3, for the state of the room, not the full char.
	char getDirectionalConnectivity(Point pos, char Dir);
	char getDirectionalConnectivity(int x, int y, char Dir) { return getDirectionalConnectivity(Point(x, y), Dir); }
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

	// Find if the cell exists in the map currently.
	bool findCell(Point pos);
	bool findCell(long x, long y) { return findCell(Point(x, y)); }

public:
	// Point Data Setters
	void setData(Point pos, int ndata, int level) { curr->setData(pos, ndata, level); }
	void setDataInCell(Point CellPos, Point ptPos, int ndata, int level);

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