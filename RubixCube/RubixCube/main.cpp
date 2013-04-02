#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>
#include <vector>
#include "Cube.h"
#include "Rotator.h"
#include "Block.h"


using namespace std;

/******************************************************************************
This is used to test the Cube class.  For details on the Cube class, start
with "Cube.h"
******************************************************************************/
int main(void)
{
	/*****************************************************************************/
	// test Rotator::difference

	Rotator tStart;
	Rotator tRotate;
	byte diff;
	
	for (int o = 0; o<6; o++) // load every possible starting Orientation
	{
		for (int x=0;x<=3;x++)
		{
			tStart.loadOrientation((Orientation)o);
			tRotate.loadOrientation(tStart.getOrientation());
			// do the rotations manually
			for (int xr=x;xr>0;xr--)
				tRotate.incXaxis();
			// compare
			diff = tStart.difference(tStart,tRotate);
			// x is the default opposite, so it is the only one that should work correctly.
			// but vectors that are pointing the direction of the orientation axis cannot be rotated.
			if ((diff != BasicBlock::getNRKey(x,0,0)) && (o!=2) && (o!=3))// != tStart)) ///)&& (tStart != tRotate.getOpposite()))
 				throw std::exception("bad Rotator::difference");

		}
		for (int y=0;y<=3;y++)
		{
			tStart.loadOrientation((Orientation)o);
			tRotate.loadOrientation(tStart.getOrientation());
			// do the rotations manually
			for (int yr=y;yr>0;yr--)
				tRotate.incYaxis();
			// compare
			diff = tStart.difference(tStart,tRotate);
			// vectors on the same axis are difficult to construct a correct test for, so ignore them.
			if ((diff!=BasicBlock::getNRKey(0,y,0)) && (tStart!=tRotate) && (tStart != tRotate.getOpposite()))
				throw std::exception("bad Rotator::difference");
		}
		for (int z=0;z<=3;z++)
		{
			tStart.loadOrientation((Orientation)o);
			tRotate.loadOrientation(tStart.getOrientation());
			// do the rotations manually
			for (int zr=z;zr>0;zr--)
				tRotate.incZaxis();
			// compare
			diff = tStart.difference(tStart,tRotate);
			// vectors on the same axis are difficult to construct a correct test for, so ignore them.
			if ((diff!=BasicBlock::getNRKey(0,0,z)) && (tStart!=tRotate) && (tStart != tRotate.getOpposite()))
				throw std::exception("bad Rotator::difference");

		}
	}


//**********************************************************************************************************/
	// Test derotator
	Block myTestBlock,myBaseBlock;
	myTestBlock.homeBlock=0;
	myTestBlock.orientationVector = orientVector(Orientation::top,Orientation::front);
	myTestBlock.position=Positions::topLeftBack;
	myBaseBlock = myTestBlock;

	for (int x=0;x<7;x++)
		for (int y=0;y<7;y++)
			for (int z=0;z<7;z++)
			{
//				myBaseBlock.orientationVector = myTestBlock.orientationVector = orientVector(Orientation::top,Orientation::front);
				rByte NRkey= BasicBlock::getNRKey(x,y,z);
				BasicBlock::rotate(myTestBlock,NRkey);
				// this should calculate the difference between the two vectors
				rByte deRot = BasicBlock::deRotate(myTestBlock.orientationVector,myBaseBlock.orientationVector);
				// this should rotate the cube back to where it started
				BasicBlock::rotate(myBaseBlock,deRot);
				if (!(myBaseBlock.orientationVector == myTestBlock.orientationVector))
					throw std::exception("bad derotator");
			}




	

	Cube myCube;
	myCube.SetCubeToSolved();
	int test = Cube::unSolved(myCube);
	std::cout << "Show cube with block numbers\n";
	std::cout << myCube.toString(true);
	std::cout << "Show cube without block numbers\n";
	std::cout << myCube.toString(false);

	
	std::cout << "Show topClockwise move\n";
	myCube.move((size_t) Moves::topClockwise);
	std::cout << myCube.toString(false);

	std::cout << "Show topclockwise then topCounterClockwise move\n";
	myCube.move((size_t) Moves::topCounterClockwise);
	std::cout << myCube.toString(false);


	std::cout << "Show topCounterClockwise move\n";
	myCube.SetCubeToSolved();
	myCube.move((size_t) Moves::topCounterClockwise);
	std::cout << myCube.toString(false);

	std::cout << "Show bottomClockwise move\n";
	myCube.SetCubeToSolved();
	myCube.move((size_t) Moves::bottomClockwise);
	std::cout << myCube.toString(false);

	std::cout << "Show bottomCounterClockwise move\n";
	myCube.SetCubeToSolved();
	myCube.move((size_t) Moves::bottomCounterClockwise);
	std::cout << myCube.toString(false);

	std::cout << "Show rightClockwise move\n";
	myCube.SetCubeToSolved();
	myCube.move((size_t) Moves::rightClockwise);
	std::cout << myCube.toString(false);

	std::cout << "Show rightCounterClockwise move\n";
	myCube.SetCubeToSolved();
	myCube.move((size_t) Moves::rightCounterClockwise);
	std::cout << myCube.toString(false);

	std::cout << "Show leftClockwise move\n";
	myCube.SetCubeToSolved();
	myCube.move((size_t) Moves::leftClockwise);
	std::cout << myCube.toString(false);

	std::cout << "Show leftCounterClockwise move\n";
	myCube.SetCubeToSolved();
	myCube.move((size_t) Moves::leftCounterClockwise);
	std::cout << myCube.toString(false);

	std::cout << "Show frontClockwise move\n";
	myCube.SetCubeToSolved();
	myCube.move((size_t) Moves::frontClockwise);
	std::cout << myCube.toString(false);

	std::cout << "Show frontCounterClockwise move\n";
	myCube.SetCubeToSolved();
	myCube.move((size_t) Moves::frontCounterClockwise);
	std::cout << myCube.toString(false);

	std::cout << "Show backClockwise move\n";
	myCube.SetCubeToSolved();
	myCube.move((size_t) Moves::backClockwise);
	std::cout << myCube.toString(false);

	std::cout << "Show backCounterClockwise move\n";
	myCube.SetCubeToSolved();
	myCube.move((size_t) Moves::backCounterClockwise);
	std::cout << myCube.toString(false);

	std::cout << "Show move combination\n";
	std::cout << "2RC,2LC,2TC,2BoC,2FC,2BaC \n";
	myCube.SetCubeToSolved();
	myCube.move((size_t) Moves::rightClockwise);
	myCube.move((size_t) Moves::rightClockwise);
	myCube.move((size_t) Moves::leftClockwise);
	myCube.move((size_t) Moves::leftClockwise);
	myCube.move((size_t) Moves::topClockwise);
	myCube.move((size_t) Moves::topClockwise);
	myCube.move((size_t) Moves::bottomClockwise);
	myCube.move((size_t) Moves::bottomClockwise);
	myCube.move((size_t) Moves::frontClockwise);
	myCube.move((size_t) Moves::frontClockwise);
	myCube.move((size_t) Moves::backClockwise);
	myCube.move((size_t) Moves::backClockwise);
	std::cout << myCube.toString(false);

	test = Cube::unSolved(myCube);

	myCube.calculateAdvancedTransferFunctions();

	system("pause");
}
