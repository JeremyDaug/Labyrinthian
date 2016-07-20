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

void Cell::updateWallsToConnectivity()
{
	// run through each room assuming it's connectivity is true.
	for (int x = 0; x < 8; ++x)
	{
		for (int y = 0; y < 8; ++y)
		{
			// don't bother checking the walls, just update them regardless.
			updateWallsToRoom(Point(x, y));
		}
	}
}

void Cell::updateWallsToRoom(Point room)
{
	int x = room.x;
	int y = room.y;
	ConnStat N = getDirectionalConnectivity(Point(x, y), north);
	ConnStat E = getDirectionalConnectivity(Point(x, y), east);
	ConnStat S = getDirectionalConnectivity(Point(x, y), south);
	ConnStat W = getDirectionalConnectivity(Point(x, y), west);
	// 1-6 because we ignore the corners. may update to include them later.
	for (int roomX = 1; roomX < 7; ++roomX)
	{
		// N
		if (N == closed)
		{
			setData(Point(x * 8 + roomX, 8 * y + 7), 1, 0);
		}
		else if (N == open)
		{
			setData(Point(x * 8 + roomX, 8 * y + 7), 0, 0);
		}
		else if (N == locked)
		{
			if (roomX == 3 || roomX == 4)
				setData(Point(x * 8 + roomX, 8 * y + 7), 2, 0);
			else
				setData(Point(x * 8 + roomX, 8 * y + 7), 1, 0);
		}
		else if (N == unlocked)
		{
			if (roomX == 3 || roomX == 4)
				setData(Point(x * 8 + roomX, 8 * y + 7), 3, 0);
			else
				setData(Point(x * 8 + roomX, 8 * y + 7), 1, 0);
		}
		// S
		if (S == closed)
		{
			setData(Point(x * 8 + roomX, 8 * y), 1, 0);
		}
		else if (S == open)
		{
			setData(Point(x * 8 + roomX, 8 * y), 0, 0);
		}
		else if (S == locked)
		{
			if (roomX == 3 || roomX == 4)
				setData(Point(x * 8 + roomX, 8 * y), 2, 0);
			else
				setData(Point(x * 8 + roomX, 8 * y), 1, 0);
		}
		else if (S == unlocked)
		{
			if (roomX == 3 || roomX == 4)
				setData(Point(x * 8 + roomX, 8 * y), 3, 0);
			else
				setData(Point(x * 8 + roomX, 8 * y), 1, 0);
		}
	}
	for (int roomY = 1; roomY < 7; ++roomY)
	{

		// E
		if (E == closed)
		{
			setData(Point(x * 8 + 7, 8 * y + roomY), 1, 0);
		}
		else if (E == open)
		{
			setData(Point(x * 8 + 7, 8 * y + roomY), 0, 0);
		}
		else if (E == locked)
		{
			if (roomY == 3 || roomY == 4)
				setData(Point(x * 8 + 7, 8 * y + roomY), 2, 0);
			else
				setData(Point(x * 8 + 7, 8 * y + roomY), 1, 0);
		}
		else if (E == unlocked)
		{
			if (roomY == 3 || roomY == 4)
				setData(Point(x * 8 + 7, 8 * y + roomY), 3, 0);
			else
				setData(Point(x * 8 + 7, 8 * y + roomY), 1, 0);
		}
		// W
		if (W == closed)
		{
			setData(Point(x * 8, 8 * y + roomY), 1, 0);
		}
		else if (W == open)
		{
			setData(Point(x * 8, 8 * y + roomY), 0, 0);
		}
		else if (W == locked)
		{
			if (roomY == 3 || roomY == 4)
				setData(Point(x * 8, 8 * y + roomY), 2, 0);
			else
				setData(Point(x * 8, 8 * y + roomY), 1, 0);
		}
		else if (W == unlocked)
		{
			if (roomY == 3 || roomY == 4)
				setData(Point(x * 8, 8 * y + roomY), 3, 0);
			else
				setData(Point(x * 8, 8 * y + roomY), 1, 0);
		}
	}
}

bool Cell::checkRoomConnections(Point pos)
{
	Point n(pos.x, pos.y + 1), e(pos.x + 1, pos.y), s(pos.x, pos.y - 1), w(pos.x - 1, pos.y);
	if (n.y < 8)
	{
		if ( (getConnectivity(n) != 0 || getDirectionalConnectivity(pos, north) != locked) ||
			 (getConnectivity(pos) != 0 || getDirectionalConnectivity(n, south) != locked) )
		{
			if (getDirectionalConnectivity(n, south) != getDirectionalConnectivity(pos, north))
				return false;
		}
	}
	if (e.x < 8)
	{
		if ( (getConnectivity(e) != 0 || getDirectionalConnectivity(pos, east) != locked) ||
			 (getConnectivity(pos) != 0 || getDirectionalConnectivity(e, west) != locked) )
		{
			if (getDirectionalConnectivity(e, west) != getDirectionalConnectivity(pos, east))
				return false;
		}
	}
	if (s.y >= 0)
	{
		if ( (getConnectivity(s) != 0 || getDirectionalConnectivity(pos, south) != locked) ||
			 (getConnectivity(pos) != 0 || getDirectionalConnectivity(s, north) != locked) )
		{
			if (getDirectionalConnectivity(s, north) != getDirectionalConnectivity(pos, south))
				return false;
		}
	}
	if (w.x >= 0)
	{
		if ( (getConnectivity(w) != 0 || getDirectionalConnectivity(pos, west) != locked) ||
			 (getConnectivity(pos) != 0 || getDirectionalConnectivity(w, east) != locked) )
		{
			if (getDirectionalConnectivity(w, east) != getDirectionalConnectivity(pos, west))
				return false;
		}
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
					data[i][j][k] = 1; // wall.
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
