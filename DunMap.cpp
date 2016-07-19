#include "DunMap.h"

void DunMap::SetRoomConnections(Point& cell, Point& room, Direction dir, ConnStat connType)
{
	Direction undir;
	Point adj;
	if (dir == north)
	{
		undir = south;
		adj = Point(room.x, room.y + 1);
	}
	else if (dir == east)
	{
		undir = west;
		adj = Point(room.x + 1, room.y);
	}
	else if (dir == south)
	{
		undir = north;
		adj = Point(room.x, room.y - 1);
	}
	else if (dir == west)
	{
		undir = east;
		adj = Point(room.x - 1, room.y);
	}
	if (!(0 <= adj.x && adj.x < 8 && 0 <= adj.y && adj.y < 8))
	{
		Point adjCell;
		if (adj.y > 7) // north
		{
			adjCell = Point(cell.x, cell.y + 1);
			adj = Point(adj.x, 0);
		}
		else if (adj.x > 7) // east
		{
			adjCell = Point(cell.x + 1, cell.y);
			adj = Point(0, adj.y);
		}
		else if (adj.y < 0) // South
		{
			adjCell = Point(cell.x, cell.y - 1);
			adj = Point(adj.x, 7);
		}
		else if (adj.x < 0) // west
		{
			adjCell = Point(cell.x - 1, cell.y);
			adj = Point(0, adj.y);
		}

		// check if cell exists.
		if (!CellExists(adjCell))
		{
			BigMap[adjCell] = new Cell;
			existingCells.push_back(adjCell);
		}
	}
}

bool DunMap::InterCellConnectivityCheck(Point QuestionCell)
{
	/*
	Check that all connections that cross cell lines are valid.
	If a cell does not exist assume all of it's intercell connections are true.
	*/
	Cell* Nptr = nullptr;
	Cell* Sptr = nullptr;
	Cell* Eptr = nullptr;
	Cell* Wptr = nullptr;
	if (BigMap.find(Point(QuestionCell.x, QuestionCell.y + 1)) != BigMap.end())
		Cell* N = BigMap[Point(QuestionCell.x, QuestionCell.y + 1)];

	if (BigMap.find(Point(QuestionCell.x, QuestionCell.y - 1)) != BigMap.end())
		Cell* S = BigMap[Point(QuestionCell.x, QuestionCell.y - 1)];

	if (BigMap.find(Point(QuestionCell.x + 1 , QuestionCell.y)) != BigMap.end())
		Cell* E = BigMap[Point(QuestionCell.x + 1, QuestionCell.y)];

	if (BigMap.find(Point(QuestionCell.x - 1, QuestionCell.y)) != BigMap.end())
		Cell* W = BigMap[Point(QuestionCell.x - 1, QuestionCell.y)];

	// get the current cell.
	Cell* CurrCell = nullptr;
	if (BigMap.find(Point(QuestionCell.x, QuestionCell.y)) != BigMap.end())
		CurrCell = BigMap[Point(QuestionCell.x, QuestionCell.y)];
	else
		return true;

	for (int i = 0; i < 8; ++i)
	{
		if (Nptr)
		{
			if (Nptr->getDirectionalConnectivity(i, 0, south) != CurrCell->getDirectionalConnectivity(i, 7, north))
				return false;

		}
		if (Sptr)
		{
			if (Sptr->getDirectionalConnectivity(i, 7, north) != CurrCell->getDirectionalConnectivity(i, 0, south))
				return false;
		}
		if (Eptr)
		{
			if (Eptr->getDirectionalConnectivity(0, i, west) != CurrCell->getDirectionalConnectivity(7, i, east))
				return false;
		}
		if (Wptr)
		{
			if (Wptr->getDirectionalConnectivity(7, i, east) != CurrCell->getDirectionalConnectivity(0, i, west))
				return false;
		}
	}

	return true;
}

bool DunMap::CellExists(Point& pos)
{
	for (unsigned int i = 0; i < existingCells.size(); ++i)
	{
		if (existingCells[i] == pos)
			return true;
	}
	return false;
}

void DunMap::setDataInCell(Point& CellPos, Point& ptPos, int ndata, int level)
{
	if( BigMap.find(CellPos) != BigMap.end())
		BigMap[CellPos]->setData(ptPos, ndata, level);
}

DunMap::DunMap()
{
	// create default current cell.
	curr = new Cell;

	BigMap[Point(0, 0)] = curr;
	currPos = Point(0, 0);

	// create default nearby cells.
	for (int i = 0; i < 9; ++i)
		near[i] = new Cell;

	// Existing Cells vector
	for (int i = -1; i < 2; ++i)
	{
		for (int j = -1; j < 2; ++j)
		{
			existingCells.push_back(Point(i, j));
		}
	}

	// fill the map with Near Cells
	BigMap[Point(0, 1)] = near[0];
	BigMap[Point(1, 1)] = near[1];
	BigMap[Point(1, 0)] = near[2];
	BigMap[Point(1, -1)] = near[3];
	BigMap[Point(0, -1)] = near[4];
	BigMap[Point(-1, -1)] = near[5];
	BigMap[Point(-1, 0)] = near[6];
	BigMap[Point(-1, 1)] = near[7];
}

DunMap::~DunMap()
{
	// Delete all in the Map
	for (std::map<Point, Cell*>::iterator it = BigMap.begin();
		it != BigMap.end(); ++it)
	{
		delete it->second;
	}

	// These Cells are already deleted, so simply set them to nullptr.
	curr = nullptr;
	for (int i = 0; i < 8; ++i)
	{
		near[i] = nullptr;
	}
}

bool DunMap::CreateRoomConnectionInCell(Point& cell, Point& room, Direction dir, ConnStat connType)
{
	return false;
}

bool DunMap::CreateCell(Point& pos)
{
	// check that the cell doesnt' already exist.
	if (!CellExists(pos))
		return false;
	// else get create the cell.
	existingCells.push_back(pos);
	BigMap[pos] = new Cell;
	return true;
}

bool DunMap::CreateBlankRoom(Point& pos)
{
	Point testCell = Point(0, 0);
	if(!CreateRoomConnection(pos, north, unlocked))
		return false; // North is unlocked
	SetRoomConnections(testCell, pos, east, locked);// east is locked
	SetRoomConnections(testCell, pos, south, open);// south is open
	SetRoomConnections(testCell, pos, west, closed);// west is closed
	return true;
}

bool DunMap::CreateBlankRoomInCell(Point& cell, Point room)
{
	return false;
}

bool DunMap::ConnectivityConsistencyCheck()
{
	for (unsigned int i = 0; i < existingCells.size(); ++i)
		if (!InterCellConnectivityCheck(existingCells[i]))
			return false;
	return true;
}
