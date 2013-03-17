#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>
#include <vector>
#include "Cube.h"

/******************************************************************************
This is used to test the Cube class.  For details on the Cube class, start
with "Cube.h"
******************************************************************************/
void rotate(int axis, int times, Orientation &newTopAxis, Orientation &newFrontAxis);
void FindAllOrientations();

int main(void)
{
	Cube rootCube;

	rootCube.addBlock(topLeftFront,none,none,none,none,none,none);


	FindAllOrientations();
	system("pause");
}

// I'm making a change here to test that this will actually publish to Alex

// I'm confused about orientation.
// there are 6 faces.
// 2 face vectors are required to represent an orientation
// there are 24 orientations
// However, there are 64 paths to those 24 orientations
// I can't find a pattern that I can see, so I'm writing this to sort all of the orientations
// home Orientation is [top,front]
// tVector represents the main face/vector, and fVector represents the second face/vector.
// I'm suspecting that a matrix is the way to derive the hash function from the 64 paths to the 
// 24 orientations

struct tector
{
	int x;
	int y;
	int z;

	tector(char xi, char yi, char zi)
	{
		x=xi;
		y=yi;
		z=zi;
	}

	// all rotations are clockwise facing their positive axis
	// counterclockwise would be 3 rotations on one axis
	// x is positive facing right
	// y is positive facing top
	// z is positive facing front
	void incXaxis()
	{
		if ((x==1)||(x==-1)) {}//do nothing
		else if ((y == 1)&&(z == 0)) {y= 0;z=-1;}
		else if ((y == 0)&&(z ==-1)) {y=-1;z= 0;}
		else if ((y ==-1)&&(z == 0)) {y= 0;z= 1;}
		else if ((y == 0)&&(z == 1)) {y= 1;z= 0;}
	}

	void incYaxis()
	{
		if ((y==1)||(y==-1)) {}//do nothing
		else if ((x == 1)&&(z == 0)) {x= 0;z= 1;}
		else if ((x == 0)&&(z == 1)) {x=-1;z= 0;}
		else if ((x ==-1)&&(z == 0)) {x= 0;z=-1;}
		else if ((x == 0)&&(z ==-1)) {x= 1;z= 0;}
	}

	void incZaxis()
	{
		if ((z==1)||(z==-1)) {}//do nothing
		else if ((x == 0)&&(y == 1)) {x= 1;y= 0;}
		else if ((x == 1)&&(y == 0)) {x= 0;y=-1;}
		else if ((x == 0)&&(y ==-1)) {x=-1;y= 0;}
		else if ((x ==-1)&&(y == 0)) {x= 0;y= 1;}
	}

	Orientation getOrientation()
	{
		if ((x== 0) && (y== 1) && (z== 0)) return top;
		if ((x== 0) && (y==-1) && (z== 0)) return bottom;
		if ((x==-1) && (y== 0) && (z== 0)) return left;
		if ((x== 1) && (y== 0) && (z== 0)) return right;
		if ((x== 0) && (y== 0) && (z== 1)) return front;
		if ((x== 0) && (y== 0) && (z==-1)) return back;
		return invalid;
	}

	std::string toString()
	{
		Orientation o = getOrientation();
		if (o==top)		return "top";
		if (o==bottom)	return "bottom";
		if (o==left)	return "left";
		if (o==right)	return "right";
		if (o==front)	return "front";
		if (o==back)	return "back";
		return "invalid";
	}
};




// 0 = no rotation 
// 1 = one rotation clockwise
// 2 = two rotations clockwise (or two rotations counterclockwise)
// 3 = one rotation counterclockwise


// resolve all orientations and output them to a comma delimited file that can be 
// sorted in excel.
void FindAllOrientations()
{
	//std::ofstream resultFile;
	//resultFile.open("OrientationList.cvs");

//	resultFile << "xNum,yNum,zNum,tVector,fVector" << std::endl;
	std::cout << "xNum,yNum,zNum,tVector,fVector" << std::endl;
	int xAxis,yAxis,zAxis;
	for (xAxis=0; xAxis <= 3; ++xAxis)
		for (yAxis=0; yAxis <=3; ++yAxis)
			for (zAxis=0; zAxis <=3; ++zAxis)
			{
				tector tVector = tector(0,1,0);  // setup the top tector
				tector fVector = tector(0,0,1);  // setup the front tector
				for (int xNum=0;xNum<xAxis;++xNum) {tVector.incXaxis();fVector.incXaxis();}
				for (int yNum=0;yNum<yAxis;++yNum) {tVector.incYaxis();fVector.incYaxis();}
				for (int zNum=0;zNum<zAxis;++zNum) {tVector.incZaxis();fVector.incZaxis();}

				// Test if rotation in a different order changes the results

				tector tVector2 = tector(0,1,0);  // setup the top tector
				tector fVector2 = tector(0,0,1);  // setup the front tector
				for (int yNum=0;yNum<yAxis;++yNum) {tVector2.incYaxis();fVector2.incYaxis();}
				for (int xNum=0;xNum<xAxis;++xNum) {tVector2.incXaxis();fVector2.incXaxis();}
				for (int zNum=0;zNum<zAxis;++zNum) {tVector2.incZaxis();fVector2.incZaxis();}

				tector tVector3 = tector(0,1,0);  // setup the top tector
				tector fVector3 = tector(0,0,1);  // setup the front tector
				for (int zNum=0;zNum<zAxis;++zNum) {tVector3.incZaxis();fVector3.incZaxis();}
				for (int yNum=0;yNum<yAxis;++yNum) {tVector3.incYaxis();fVector3.incYaxis();}
				for (int xNum=0;xNum<xAxis;++xNum) {tVector3.incXaxis();fVector3.incXaxis();}


				std::cout << xAxis << "," << yAxis << "," << zAxis << "," << 
					tVector.toString()  << "," << fVector.toString() << "," << 
					tVector2.toString()  << "," << fVector2.toString() << "," << 					
					tVector3.toString()  << "," << fVector3.toString() << std::endl;
			}
	//resultFile.close();
	
}

/*************** Damn! order does matter ***********************
***  The only good news is that physical measurements do     ***
***  confirm that the above function does correctly rotate   ***
***  the cube                                                ***
***************************************************************/

