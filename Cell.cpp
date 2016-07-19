#include "Cell.h"

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

