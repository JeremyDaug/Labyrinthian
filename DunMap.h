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
	// getter of specific connectivity
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
	Cell** near[8];

	// sanity checker for whether a cell exists or not. probably need a better way.
	std::vector<Point> existingCells;

    // The Map itself.
	std::map<Point, Cell*> BigMap;

    // Functions
	bool InterCellConnectivityCheck(Point QuestionCell);

public:
	// Point Data Setters
	void setData(Point pos, int ndata, int level) { curr->setData(pos, ndata, level); }
	void setDataInCell(Point CellPos, Point ptPos, int ndata, int level);
	// tester functions
	bool ConnectivityConsistencyCheck();
	// Pathing check.
};

#endif // !DUNMAP_H