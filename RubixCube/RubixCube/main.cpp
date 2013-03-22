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
	Cube rootCube;
	rootCube.addBlock(topLeftFront,none,none,none,none,none,none);

	// for now lets create a block for testing that matches the colors of the cube itself.
	Block testBlock(topLeftFront,Faces(blue,green,orange,red,white,yellow));

	// I want to see if multiple series of rotations using my rotate function come up
	// with the same result as if the entire set was executed in sequence.

	// define and rotate a set manually for xAxis+2, yAxis+1, zAxis+3
	Rotator tManualVector = Rotator(0,1,0);  // setup the top rotator
	Rotator fManualVector = Rotator(0,0,1);  // setup the front rotator
	for (int xNum=0;xNum<2;++xNum) {tManualVector.incXaxis();fManualVector.incXaxis();}
	for (int yNum=0;yNum<1;++yNum) {tManualVector.incYaxis();fManualVector.incYaxis();}
	for (int zNum=0;zNum<3;++zNum) {tManualVector.incZaxis();fManualVector.incZaxis();}
	// rotate testBlock the same way
	rByte newKey = testBlock.rotate(0,testBlock.getRKey(2,1,3));
	orientVector testOrient = testBlock.getOrientation(testBlock.getOKey(newKey));
	if ((tManualVector==testOrient.tVector) && (fManualVector==testOrient.fVector))
	{
		std::cout << "they match for simple case" << endl;
	}
	else
	{
		std::cout << "they don't match for simple case"<< endl;
	}

	// make a single additional rotation
	{tManualVector.incXaxis();fManualVector.incXaxis();}
	rByte newKey2 = testBlock.rotate(newKey,testBlock.getRKey(1,0,0));
	testOrient = testBlock.getOrientation(testBlock.getOKey(newKey));
	if ((tManualVector==testOrient.tVector) && (fManualVector==testOrient.fVector))
	{
		std::cout << "they match for simple case"<< endl;
	}
	else
	{
		std::cout << "they don't match for simple case"<< endl;
	}


	// make a messy rotation again
	for (int xNum=0;xNum<1;++xNum) {tManualVector.incXaxis();fManualVector.incXaxis();}
	for (int yNum=0;yNum<3;++yNum) {tManualVector.incYaxis();fManualVector.incYaxis();}
	for (int zNum=0;zNum<2;++zNum) {tManualVector.incZaxis();fManualVector.incZaxis();}
	rByte newKey3 = testBlock.rotate(newKey,testBlock.getRKey(1,3,2));
	testOrient = testBlock.getOrientation(testBlock.getOKey(newKey));
	if ((tManualVector==testOrient.tVector) && (fManualVector==testOrient.fVector))
	{
		std::cout << "they match for hard case";
	}
	else
	{
		std::cout << "they don't match for hard case";
	}

	system("pause");
}
