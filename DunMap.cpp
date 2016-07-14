#include "DunMap.h"

bool Cell::roomConnValid(Point room, char dir)
{
	Point adj;
	char undir; // the opposite direction
	if (dir == north)
	{
		adj.x = room.x;
		adj.y = room.y + 1;
		undir = south;
	}
	if (dir == east)
	{
		adj.x = room.x + 1;
		adj.y = room.y;
		undir = west;
	}
	if (dir == south)
	{
		adj.x = room.x;
		adj.y = room.y - 1;
		undir = north;
	}
	if (dir == west)
	{
		adj.x = room.x - 1;
		adj.y = room.y;
		undir = east;
	}
	// if inside the cell do the check. else return True.
	if ((0 <= adj.x) &&
		(0 <= adj.y) &&
		(adj.x < 8)  &&
		(adj.y < 8) )
	{
		return (
			connMask(getConnectivity(room), dir) == connMask(getConnectivity(adj), undir)// check if connections are equal
			|| 
			(getConnectivity(room) == 0 && connMask(getConnectivity(adj), undir) == 2) // If one is unmade and the other is locked
			||
			(connMask(getConnectivity(room), dir) == 2 && getConnectivity(adj) == 0)// or the other way around.
			);
	}
	return true;
}

char Cell::reverseMask(char dir, ConnStat type)
{
	if (dir == north)
		return type * 64;
	else if (dir == east)
		return type * 16;
	else if (dir == south)
		return type * 4;
	else if (dir == west)
		return type;
	else
		return 0;
	return 0;
}

bool Cell::ConnectivityCheck()
{
	// TODO check this hardcore. Seriously. It. Is. Important.
	char temp = 0;
	for (int i = 0; i < 8; ++i)
	{
		for (int j = 0; j < 8; ++j)
		{
			// Begin checks
			// N
			if (j + 1 < 8 && !roomConnValid(Point(i,j), north))
					return false;
			// E
			if (i + 1 < 8 && !roomConnValid(Point(i,j), east))
					return false;
			// S
			if (j - 1 >= 0 && !roomConnValid(Point(i,j), south))
					return false;
			// W
			if (i - 1 >= 0 && !roomConnValid(Point(i,j), west))
					return false;
		}
	}
	return true;
}

Cell::Cell()
{
	// fill the data with standard values.
	for (int i = 0; i < 64; ++i)
	{
		for (int j = 0; j < 64; ++j)
		{
			for (int k = 0; k < 5; ++k)
			{
				// if on the edge of the room, mark as generic wall (1)
				if (((i % 8) == 0 || (i % 8) == 7 ||
					(j % 8) == 0 || (j % 8) == 7) && k == 0)
				{
					if ((i % 8) == 3 || (i % 8) == 4
						|| (j % 8) == 3 || (j % 8) == 4)
					{
						data[i][j][k] = 2; // locked door
					}
					else
					{
						data[i][j][k] = 1; // wall.
					}
				}
				else // mark it as a blank tile (0)
				{
					data[i][j][k] = 0;
				}
			}
		}
	}

	for (int i = 0; i < 8; ++i)
	{
		for (int j = 0; j < 8; ++j)
		{
			connectivity[i][j] = 0;
		}
	}
}

void Cell::setConnectivity(Point pos, char Dir, ConnStat type)
{
	if (0 <= pos.x && pos.x < 8 && 0 <= pos.y && pos.y < 8)
	{
		char connStatus = connectivity[pos.x][pos.y];
		if (Dir == north)
		{
			connStatus -= connStatus | (64 + 128);
			connStatus += reverseMask(Dir, type) * 64;
		}
		else if (Dir == east)
		{
			connStatus -= connStatus | (16 + 32);
			connStatus += reverseMask(Dir, type) * 16;
		}
		else if (Dir == south)
		{
			connStatus -= connStatus | (4 + 8);
			connStatus += reverseMask(Dir, type) * 4;
		}
		else if (Dir == west)
		{
			connStatus -= connStatus | (1 + 2);
			connStatus += reverseMask(Dir, type);
		}
		
		connectivity[pos.x][pos.y] = connStatus;
	}
}

ConnStat Cell::connMask(char connectivity, char dir)
{
	if (dir == north)
		return ConnStat(((64 + 128) | connectivity) / 64);
	else if (dir == east)
		return ConnStat(((16 + 32) | connectivity) / 16);
	else if (dir == south)
		return ConnStat(((4 + 8) | connectivity) / 4);
	else if (dir == west)
		return ConnStat((1 + 2) | connectivity);
	else
		return ConnStat(0);
}

ConnStat Cell::getDirectionalConnectivity(Point pos, char Dir)
{
	char connect = getConnectivity(pos);
	return connMask(connect, Dir);
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

bool DunMap::CellExists(Point pos)
{
	for (unsigned int i = 0; i < existingCells.size(); ++i)
	{
		if (existingCells[i] == pos)
			return true;
	}
	return false;
}

void DunMap::setDataInCell(Point CellPos, Point ptPos, int ndata, int level)
{
	if( BigMap.find(CellPos) != BigMap.end())
		BigMap[CellPos]->setData(ptPos, ndata, level);
}

DunMap::DunMap()
{
	// create default current cell.
	curr = new Cell;

	BigMap[Point(0, 0)] = curr;

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

bool DunMap::CreateCell(Point pos)
{
	// check that the cell doesnt' already exist.
	if (!CellExists(pos))
		return false;
	// else get create the cell.
	existingCells.push_back(pos);
	BigMap[pos] = new Cell;
	return true;
}

bool DunMap::ConnectivityConsistencyCheck()
{
	for (unsigned int i = 0; i < existingCells.size(); ++i)
		if (!InterCellConnectivityCheck(existingCells[i]))
			return false;
	return true;
}
