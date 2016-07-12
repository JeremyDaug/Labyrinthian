#include "DunMap.h"

bool Cell::ConnectivityCheck()
{
	// TODO check this hardcore. Seriously. It. Is. Important.
	char temp = 0;
	for (int i = 0; i < 8; ++i)
	{
		for (int j = 0; j < 8; ++j)
		{
			// get current room
			char x = getConnectivity(i, j);

			char e = getConnectivity(i + 1, j);
			char s = getConnectivity(i, j - 1);
			char w = getConnectivity(i - 1, j);
			// Begin checks
			// N
			if (j + 1 < 8)
			{
				char n = getConnectivity(i, j + 1);
				if ((connMask(x, 'N') != connMask(n, 'S')) || // if connectivity isn't the same.
					!(x == 0 && connMask(n, 'S') == 2) || // The current room doesn't exist and the other is locked
					!(n == 0 && connMask(x, 'N'))) // or the other room DNE and current room is locked.
					return false;
			}
			// E
			if (i + 1 < 8)
			{
				char e = getConnectivity(i + 1, j);
				if ((connMask(x, 'E') != connMask(e, 'W')) || // if connectivity isn't the same.
					!(x == 0 && connMask(e, 'W') == 2) || // The current room doesn't exist and the other is locked
					!(e == 0 && connMask(x, 'E'))) // or the other room DNE and current room is locked.
					return false;
			}
			// s
			if (j - 1 >= 0)
			{
				char s = getConnectivity(i, j - 1);
				if ((connMask(x, 'S') != connMask(s, 'N')) || // if connectivity isn't the same.
					!(x == 0 && connMask(s, 'N') == 2) || // The current room doesn't exist and the other is locked
					!(s == 0 && connMask(x, 'S'))) // or the other room DNE and current room is locked.
					return false;
			}
			// w
			if (i - 1 >= 0)
			{
				char w = getConnectivity(i - 1, j);
				if ((connMask(x, 'W') != connMask(w, 'E')) || // if connectivity isn't the same.
					!(x == 0 && connMask(w, 'E') == 2) || // The current room doesn't exist and the other is locked
					!(w == 0 && connMask(x, 'W'))) // or the other room DNE and current room is locked.
					return false;
			}
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

char Cell::connMask(char connectivity, char dir)
{
	char w = connectivity % 4;
	char s = ((connectivity - w) % 16) / 4;
	char e = ((connectivity - w - s) % 64) / 16;
	char n = ((connectivity - w - s - e) % 256) / 64;
	switch (dir)
	{
	case 'N':
		return n;
	case 'E':
		return e;
	case 'S':
		return s;
	case 'W':
		return w;
	}
	return 0;
}

char Cell::getDirectionalConnectivity(Point pos, char Dir)
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
			if (Nptr->getDirectionalConnectivity(i, 0, 'S') != CurrCell->getDirectionalConnectivity(i, 7, 'N'))
				return false;

		}
		if (Sptr)
		{
			if (Sptr->getDirectionalConnectivity(i, 7, 'N') != CurrCell->getDirectionalConnectivity(i, 0, 'S'))
				return false;
		}
		if (Eptr)
		{
			if (Eptr->getDirectionalConnectivity(0, i, 'W') != CurrCell->getDirectionalConnectivity(7, i, 'E'))
				return false;
		}
		if (Wptr)
		{
			if (Wptr->getDirectionalConnectivity(7, i, 'E') != CurrCell->getDirectionalConnectivity(0, i, 'W'))
				return false;
		}
	}

	return true;
}

bool DunMap::findCell(Point pos)
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
	if (!findCell(pos))
		return false;
	// else get create the cell.
	existingCells.push_back(pos);
	BigMap[pos] = new Cell;
	return true;
}

bool DunMap::ConnectivityConsistencyCheck()
{

}
