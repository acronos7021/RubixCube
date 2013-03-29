#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>
#include <vector>
#include "Cube.h"
#include "Rotator.h"

using namespace std;

/******************************************************************************
This is used to test the Cube class.  For details on the Cube class, start
with "Cube.h"
******************************************************************************/
int main(void)
{
	Cube myCube;
	myCube.SetCubeToSolved();
	int test = Cube::unSolved(myCube);
	std::cout << "Show cube with block numbers\n";
	std::cout << myCube.toString(true);
	std::cout << "Show cube without block numbers\n";
	std::cout << myCube.toString(false);

	
	std::cout << "Show topClockwise move\n";
	myCube.move(topClockwise);
	std::cout << myCube.toString(false);

	std::cout << "Show topclockwise then topCounterClockwise move\n";
	myCube.move(topCounterClockwise);
	std::cout << myCube.toString(false);


	std::cout << "Show topCounterClockwise move\n";
	myCube.SetCubeToSolved();
	myCube.move(topCounterClockwise);
	std::cout << myCube.toString(false);

	std::cout << "Show bottomClockwise move\n";
	myCube.SetCubeToSolved();
	myCube.move(bottomClockwise);
	std::cout << myCube.toString(false);

	std::cout << "Show bottomCounterClockwise move\n";
	myCube.SetCubeToSolved();
	myCube.move(bottomCounterClockwise);
	std::cout << myCube.toString(false);

	std::cout << "Show rightClockwise move\n";
	myCube.SetCubeToSolved();
	myCube.move(rightClockwise);
	std::cout << myCube.toString(false);

	std::cout << "Show rightCounterClockwise move\n";
	myCube.SetCubeToSolved();
	myCube.move(rightCounterClockwise);
	std::cout << myCube.toString(false);

	std::cout << "Show leftClockwise move\n";
	myCube.SetCubeToSolved();
	myCube.move(leftClockwise);
	std::cout << myCube.toString(false);

	std::cout << "Show leftCounterClockwise move\n";
	myCube.SetCubeToSolved();
	myCube.move(leftCounterClockwise);
	std::cout << myCube.toString(false);

	std::cout << "Show frontClockwise move\n";
	myCube.SetCubeToSolved();
	myCube.move(frontClockwise);
	std::cout << myCube.toString(false);

	std::cout << "Show frontCounterClockwise move\n";
	myCube.SetCubeToSolved();
	myCube.move(frontCounterClockwise);
	std::cout << myCube.toString(false);

	std::cout << "Show backClockwise move\n";
	myCube.SetCubeToSolved();
	myCube.move(backClockwise);
	std::cout << myCube.toString(false);

	std::cout << "Show backCounterClockwise move\n";
	myCube.SetCubeToSolved();
	myCube.move(backCounterClockwise);
	std::cout << myCube.toString(false);

	std::cout << "Show move combination\n";
	std::cout << "2RC,2LC,2TC,2BoC,2FC,2BaC \n";
	myCube.SetCubeToSolved();
	myCube.move(rightClockwise);
	myCube.move(rightClockwise);
	myCube.move(leftClockwise);
	myCube.move(leftClockwise);
	myCube.move(topClockwise);
	myCube.move(topClockwise);
	myCube.move(bottomClockwise);
	myCube.move(bottomClockwise);
	myCube.move(frontClockwise);
	myCube.move(frontClockwise);
	myCube.move(backClockwise);
	myCube.move(backClockwise);
	std::cout << myCube.toString(false);

	test = Cube::unSolved(myCube);

	myCube.calculateAdvancedTransferFunctions();

	system("pause");
}
