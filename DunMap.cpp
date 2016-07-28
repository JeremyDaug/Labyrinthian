#include "DunMap.h"

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

void DunMap::CreateCell(Point& pos)
{
	if (CellExists(pos))
		return;
	// if cell doesn't exist, add it.
	existingCells.push_back(pos);
	BigMap[pos] = new Cell;
}

bool DunMap::ConnectivityConsistencyCheck()
{
	for (unsigned int i = 0; i < existingCells.size(); ++i)
	{
		// connection between cells.
		if (!InterCellConnectivityCheck(existingCells[i]))
		{
			return false;
		}
		// connections in cells.
		if (!BigMap[existingCells[i]]->ConnectivityCheck())
		{
			return false;
		}
	}
	return true;
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

	if (BigMap.find(Point(QuestionCell.x + 1, QuestionCell.y)) != BigMap.end())
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
	if (BigMap.find(CellPos) != BigMap.end())
		BigMap[CellPos]->setData(ptPos, ndata, level);
}

void DunMap::SetRoomConnectivity(Point& cell, Point& room, Direction dir, ConnStat connType)
{
	BigMap[cell]->setConnectivity(room, dir, connType);
}

// Above are static code, it should not need to be change.

void DunMap::SetMutualRoomConnectivity(Point& cell, Point& room, Direction dir, ConnStat connType)
{
	// get adjacent room and other cell if needed. Create other cell if needed as well.
	Point adj;
	Direction undir;
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

	// check if it's in the cell
	Point otherCell = cell;
	if (adj.x > 7) // to the east
	{
		++otherCell.x;
		adj.x = 0;
	}
	if (adj.x < 0) // to the west
	{
		--otherCell.x;
		adj.x = 7;
	}
	if (adj.y > 7) // to the north
	{
		++otherCell.y;
		adj.y = 0;
	}
	if (adj.y < 0) // to the south
	{
		--otherCell.y;
		adj.y = 7;
	}

	// make sure the other cell exists.
	CreateCell(otherCell);

	// now that we're ready update the rooms appropriately.
	SetRoomConnectivity(cell, room, dir, connType);
	SetRoomConnectivity(otherCell, adj, undir, connType);
}

bool DunMap::CreateRoomConnectionInCell(Point& cell, Point& room, Direction dir, ConnStat connType)
{
	// Get info for the other room, including if it's in another cell.
	Direction undir;
	Point adj;
	if (dir == north)
	{
		undir = south;
		adj = Point(room.x, room.y + 1);
	}
	if (dir == east)
	{
		undir = west;
		adj = Point(room.x + 1, room.y);
	}
	if (dir == south)
	{
		undir = north;
		adj = Point(room.x, room.y - 1);
	}
	if (dir == west)
	{
		undir = east;
		adj = Point(room.x - 1, room.y);
	}
	
	// check if it's in the cell
	Point otherCell = cell;
	if (adj.x > 7) // to the east
	{
		++otherCell.x;
		adj.x = 0;
	}
	if (adj.x < 0) // to the west
	{
		--otherCell.x;
		adj.x = 7;
	}
	if (adj.y > 7) // to the north
	{
		++otherCell.y;
		adj.y = 0;
	}
	if (adj.y < 0) // to the south
	{
		--otherCell.y;
		adj.y = 7;
	}

	// make sure the other cell exists
	CreateCell(otherCell);

	// get our connection statuses just so we don't have to call them 5 times
	ConnStat cellCon = getConnectionFor(cell, room, dir);
	ConnStat otherCellCon = getConnectionFor(otherCell, adj, undir);

	// check if the ConnType were going to is already there, if it is just return true you technically did
	// it.
	if (cellCon == connType && otherCellCon == connType)
	{
		return true;
	} // else continue and assume we need to change both. It's simpler that way.

	// check if the connection is locked to locked, unlocked to unlocked, or locked to DNE
	// if it is, delegate to toggle.
	if (connType == locked)
	{
		// if we want to lock unlocked rooms (the rooms should ALWAYS be the same) simply toggle them.
		if (cellCon == unlocked && otherCellCon == unlocked)
		{
			return toggleRoomLock(cell, room, dir);
		}
	}
	else if (connType == unlocked)
	{
		// if we want to unlock locked rooms toggle
		if (cellCon == unlocked && otherCellCon == unlocked)
		{
			return toggleRoomLock(cell, room, dir);
		}
		// if we want to unlock a locked room to nothing toggle, remember our room always exists.
		else if (cellCon == locked &&
			BigMap[otherCell]->getConnectivity(adj) == 0)
		{
			return toggleRoomLock(cell, room, dir);
		}

	}
	
	// Since we aren't simply toggling do the change. BUT only if the ConnStat's match on both rooms.
	if (cellCon == otherCellCon)
	{
		SetMutualRoomConnectivity(cell, room, dir, connType);
	}
	else
		return false;

	return true;
}

bool DunMap::toggleRoomLock(Point & cell, Point & room, Direction dir)
{
	// Get info for the other room, including if it's in another cell.
	Direction undir;
	Point adj;
	if (dir == north)
	{
		undir = south;
		adj = Point(room.x, room.y + 1);
	}
	if (dir == east)
	{
		undir = west;
		adj = Point(room.x + 1, room.y);
	}
	if (dir == south)
	{
		undir = north;
		adj = Point(room.x, room.y - 1);
	}
	if (dir == west)
	{
		undir = east;
		adj = Point(room.x - 1, room.y);
	}

	// check if it's in the cell
	Point otherCell = cell;
	if (adj.x > 7) // to the east
	{
		++otherCell.x;
		adj.x = 0;
	}
	if (adj.x < 0) // to the west
	{
		--otherCell.x;
		adj.x = 7;
	}
	if (adj.y > 7) // to the north
	{
		++otherCell.y;
		adj.y = 0;
	}
	if (adj.y < 0) // to the south
	{
		--otherCell.y;
		adj.y = 7;
	}

	// make sure the other cell exists.
	CreateCell(otherCell);

	// since your calling this we know that the connection is valid. If we are unlocking into a DNE room
	// create that room and unlock them.
	if (getConnectionFor(cell, room, dir) == locked &&
		BigMap[otherCell]->getConnectivity(adj) == 0)
	{
		CreateRoom(otherCell);
	}
	else if (getConnectionFor(cell, room, dir) == locked &&
		getConnectionFor(otherCell, adj, undir) == locked)
	{
		SetMutualRoomConnectivity(cell, room, dir, unlocked);
		return true;
	}
	else if (getConnectionFor(cell, room, dir) == unlocked &&
		getConnectionFor(otherCell, adj, undir) == unlocked)
	{
		SetMutualRoomConnectivity(cell, room, dir, locked);
		return true;
	}
	
	return false;
}

void DunMap::CreateBlankRoom(Point& room)
{
	// double check that we exist.
	CreateCell(currPos);

	// Set everything to locked.
	CreateRoomConnectionInCell(currPos, room, north, locked);
	CreateRoomConnectionInCell(currPos, room, east, locked);
	CreateRoomConnectionInCell(currPos, room, south, locked);
	CreateRoomConnectionInCell(currPos, room, west, locked);

	// update this rooms walls to the connectivity.
	updateRoomsWalls(currPos, room);
}

bool DunMap::CreateRoom(Point & room)
{
	// create a randomized room based on our current stats
	// check if it works, if not, try again.
	// once it fits, make the connections match and add some more.

	// todo actuall do that stuff. For now create a blank room.
	CreateBlankRoom(room);
	return false;
}

void DunMap::updateWalls()
{
	for (unsigned int i = 0; i < existingCells.size(); ++i)
	{
		for (int x = 0; x < 8; ++x)
		{
			for (int y = 0; y < 8; ++y)
			{
				updateRoomsWalls(existingCells[i], Point(x, y));
			}
		}
	}
}

void DunMap::updateRoomsWalls(Point& cell, Point& room)
{
	int x = room.x;
	int y = room.y;
	ConnStat N = getConnectionFor(cell, room, north);
	ConnStat S = getConnectionFor(cell, room, east);
	ConnStat E = getConnectionFor(cell, room, south);
	ConnStat W = getConnectionFor(cell, room, west);

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

// TIME TO DO SOME MOTHERFUCKING DJIKSTRAS OR SOMETHING!
/*
findPath is not interested in the fastest, or best path, it simply wants to know if there is a path
through existing rooms that does not require you to pass through a locked or closed connection.

To optimize for time we will use A* to accelerate the proccess. Once a path is found we are done, get the 
path and take that bitch home.

If we cannot find one we return an empty path.
*/
std::vector<Point> DunMap::findPath(Point& start, Point& startRoom, Point& end, Point& endRoom)
{
	std::vector<Point> retVector;
	
	return retVector;
}
