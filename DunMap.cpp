#include "DunMap.h"

int Cell::reverseMask(Direction dir, ConnStat type)
{
	if (dir == north)
	{
		return type * 64;
	}
	else if (dir == east)
	{
		return type * 16;
	}
	else if (dir == south)
	{
		return type * 4;
	}
	else if (dir == west)
	{
		return type;
	}
	else
		return 0;
}

bool Cell::checkRoomConnections(Point pos)
{
	Point n(pos.x, pos.y + 1), e(pos.x + 1, pos.y), s(pos.x, pos.y - 1), w(pos.x - 1, pos.y);
	if (n.y < 8)
	{
		if (getDirectionalConnectivity(n, south) != getDirectionalConnectivity(pos, north))
			return false;
	}
	if (e.x < 8)
	{
		if (getDirectionalConnectivity(e, west) != getDirectionalConnectivity(pos, east))
			return false;
	}
	if (s.y >= 0)
	{
		if (getDirectionalConnectivity(s, north) != getDirectionalConnectivity(pos, south))
			return false;
	}
	if (w.x >= 0)
	{
		if (getDirectionalConnectivity(w, east) != getDirectionalConnectivity(pos, west))
			return false;
	}
	return true;
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
			if (!checkRoomConnections(Point(i, j)))
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

void Cell::setConnectivity(Point pos, Direction Dir, ConnStat type)
{
	if (0 <= pos.x && pos.x < 8 && 0 <= pos.y && pos.y < 8)
	{
		short connStatus = connectivity[pos.x][pos.y];
		if (Dir == north)
		{
			connStatus -= connStatus & Nmask;
			connStatus = reverseMask(Dir, type) + connStatus;
		}
		else if (Dir == east)
		{
			connStatus -= connStatus & Emask;
			connStatus += reverseMask(Dir, type);
		}
		else if (Dir == south)
		{
			connStatus -= connStatus & Smask;
			connStatus += reverseMask(Dir, type);
		}
		else if (Dir == west)
		{
			connStatus -= connStatus & Wmask;
			connStatus += reverseMask(Dir, type);
		}
		
		connectivity[pos.x][pos.y] = connStatus;
	}
}

ConnStat Cell::connMask(short connectivity, Direction dir)
{
	if (dir == north)
		return ConnStat((Nmask & connectivity) / 64);
	else if (dir == east)
		return ConnStat((Emask & connectivity) / 16);
	else if (dir == south)
		return ConnStat((Smask & connectivity) / 4);
	else if (dir == west)
		return ConnStat((Wmask & connectivity));
	else
		return ConnStat(0);
}

ConnStat Cell::getDirectionalConnectivity(Point& pos, Direction Dir)
{
	short connect = getConnectivity(pos);
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

bool DunMap::CreateRoom(Point pos)
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
