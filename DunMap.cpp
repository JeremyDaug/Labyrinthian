#include "DunMap.h"

bool Cell::ConnectivityCheck()
{
	for (int i = 0; i < 8; ++i)
	{
		for (int j = 0; j < 8; ++j)
		{
			// check all connections
			char n = getDirectionalConnectivity(i, j, 'N');
			char e = getDirectionalConnectivity(i, j, 'E');
			char s = getDirectionalConnectivity(i, j, 'S');
			char w = getDirectionalConnectivity(i, j, 'W');
			if (i + 1 < 8) // E
			{
				if (n != getDirectionalConnectivity(i + 1, j, 'W'))
					return false;
			}
			if (i - 1 > 0) // W
			{
				if (n != getDirectionalConnectivity(i - 1, j, 'E'))
					return false;
			}
			if (j + 1 < 8) // N
			{
				if (n != getDirectionalConnectivity(i, j + 1, 'S'))
					return false;
			}
			if (j - 1 > 0) // S
			{
				if (n != getDirectionalConnectivity(i, j - 1, 'N'))
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
				// set for locked door.
				// if on the edge of the room, mark as generic wall (1)
				if ((i == 0 || i == 7 || j == 0 || j == 7) && k == 0)
					data[i][j][k] = 1;
				else // mark it as a blank tile (0)
					data[i][j][k] = 0;
			}
		}
	}

	connectivity
}

char Cell::getDirectionalConnectivity(Point pos, char Dir)
{
	char connect = getConnectivity(pos);
	char w = connect % 4;
	char s = ((connect - w) % 16)/4;
	char e = ((connect - w - s) % 64)/16;
	char n = ((connect - w - s -e) % 256)/64;
	switch (Dir) 
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

void DunMap::setDataInCell(Point CellPos, Point ptPos, int ndata, int level)
{
	if( BigMap.find(CellPos) != BigMap.end())
		BigMap[CellPos]->setData(ptPos, ndata, level);
}

bool DunMap::ConnectivityConsistencyCheck()
{
	for (int i = 0; i < existingCells.size(); ++i)
		if (!InterCellConnectivityCheck(existingCells[i]))
			return false;
	return true;
}
