// Testing file to double check Labyrinthian work without doing a full run of the program.

#include<iostream>
#include<fstream>
#include<string>
#include "DunMap.h"

void PointTests();
void CellTests();
void DunMapTests();
void Print(std::string line);

int main() {
	std::cout << "Starting Tests... \n";
	
	// Point tests
	PointTests();

	// Cell Tests
	CellTests();

	// DunMap tests
	DunMapTests();

	// cin line to show the console rather than automatically close. VSC Baka!
	int end;
	std::cin >> end;
	
	return 0;
}

void Print(std::string line)
{
	std::cout << line << std::endl;
}

void PointTests()
{
	Print("Point Tests.");
	Point x;
	Point y = Point(0, 0);
	Point z = Point(0, 1);

	// Check == works for true;
	if (x == y)
		Print("Passed: ==");
	else
		Print("Failed: ==");

	// check == works for false;
	if (x != z)
		Print("Passed: !=");
	else
		Print("Failed: !=");
}

void CellTests()
{
	Print("Cell Tests");
	Cell x;

	// check the tiles of the room
	for (int i = 63; i >= 0; --i)
	{
		for (int j = 0; j < 64; ++j)
		{
			std::cout << x.getData(Point(j, i))[0] << ' ';
		}
		std::cout << std::endl;
	}
	
	// Check default connectivity (everything should be 0).
	if (x.ConnectivityCheck())
		Print("Passed: Default Connectivity Check");
	else
		Print("Failed: Default Connectivity Check");

	// Double check the default connectivity by printing it.
	for (int i = 7; i >= 0; --i)
	{
		for (int j = 0; j < 8; ++j)
		{
			std::cout << (unsigned int)x.getConnectivity(Point(j, i)) << ' ';
		}
		std::cout << std::endl;
	}

	// set and getData for tiles.
	x.setData(Point(2, 2), 100, 1);
	if (100 == x.getData(Point(2, 2))[1])
		Print("Passed: Get/Set tile data");
	else
		Print("Failed: Get/Set tile data");

	// get/set connectivity for a room.
	x.setConnectivity(Point(2, 2), north, unlocked);
	x.setConnectivity(Point(2, 3), south, unlocked);
	x.setConnectivity(Point(4, 4), east, open);
	x.setConnectivity(Point(5, 4), west, open);
	// output tests
	if (x.getConnectivity(Point(2, 2)) == unlocked*64)
		Print("Passed: get/set 2,2 connectivity");
	else
		Print("Failed: get/set 2,2 Connectivity");
	////////////
	if (x.getConnectivity(Point(2, 3)) == unlocked * 4)
		Print("Passed: get/set 2,3 connectivity");
	else
		Print("Failed: get/set 2,3 Connectivity");
	////////////
	if(x.getConnectivity(Point(4,4)) == (open*16))
		Print("Passed: get/set 4,4 connectivity");
	else
		Print("Failed: get/set 4,4 Connectivity");
	/////////////
	if (x.getConnectivity(Point(5, 4)) == open)
		Print("Passed: get/set 5,4 connectivity");
	else
		Print("Failed: get/set 5,4 Connectivity");

	// Check Directional Connectivity
	if (x.getDirectionalConnectivity(Point(2, 2), north) == unlocked)
		Print("Passed: 2,2 Directional connectivity");
	else
		Print("Failed: 2,2 Directional Connectivity");
	////////////
	if (x.getDirectionalConnectivity(Point(2, 3), south) == unlocked)
		Print("Passed: 2,3 Directional connectivity");
	else
		Print("Failed: 2,3 Directional Connectivity");
	////////////
	if (x.getDirectionalConnectivity(Point(4, 4), east) == open)
		Print("Passed: 4,4 Directional connectivity");
	else
		Print("Failed: 4,4  Directional Connectivity");
	/////////////
	if (x.getDirectionalConnectivity(Point(5, 4), west) == open)
		Print("Passed: 5,4 Directional connectivity");
	else
		Print("Failed: 5,4 Directional Connectivity");
		
	// check room connectivity
	if (x.checkRoomConnections(Point(2, 2)))
		Print("Passed: 2,2 connect true");
	else
		Print("Failed: 2,2 connect false");

	if (x.checkRoomConnections(Point(2, 3)))
		Print("Passed: 2,3 connect true");
	else
		Print("Failed: 2,3 connect false");

	if (x.checkRoomConnections(Point(4, 4)))
		Print("Passed: 4,4 connect true");
	else
		Print("Failed: 4,4 connect false");

	if (x.checkRoomConnections(Point(5, 4)))
		Print("Passed: 5,4 connect true");
	else
		Print("Failed: 5,4 connect false");

	// Check connectivity
	if (x.ConnectivityCheck())
		Print("Passed: Connectivity Check");
	else
		Print("Failed: Connectivity Check");

	// Double check the connectivity by printing it.
	for (int i = 7; i >= 0; --i)
	{
		for (int j = 0; j < 8; ++j)
		{
			std::cout << (unsigned int)x.getConnectivity(Point(j, i)) << ' ';
		}
		std::cout << std::endl;
	}
}

void DunMapTests()
{
	Print("...DunMap Checks...");
	// Create a dunmap
	DunMap x = DunMap();

	// test Connectivity
	if (x.ConnectivityConsistencyCheck())
		Print("Passed: Connectivity Consistency Check");
	else
		Print("Failed: Connectivity Consistency Check");

	Print("...Testing Complete...");
}