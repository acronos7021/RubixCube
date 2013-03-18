#pragma once
#include <string>
#include "Enums.h"

/***********************************************************************************
	Since cubes can rotate in 3 directions, two vectors are needed to describe the orientation.
	There are actually 24 possible orientations for each block.  There are six faces and 4 ways
	to hold each face when it is facing you.  The first vector is the home top(blue), the second 
	vector is the home front(white).  Rotate is used a lot so I need it to be fast.  I do this
	using 3 variables to represent the three axis that it can rotate around and the amount in 
	each direction.  Each variable would be from 0-3 with 0 being the home position.  
	I use 6 bits of a 8 bit integer to represent the orientation using 2 bits for each direction.
	I just do a normal add of the original orientation and and offset for the rotation amount.
	

***********************************************************************************/




// there are 6 faces.
// 2 face vectors are required to represent an orientation
// there are 24 orientations
// However, there are 64 paths to those 24 orientations
// I can't find a pattern that I can see, so I'm writing this to sort all of the orientations
// home Orientation is [top,front]
// tVector represents the main face/vector, and fVector represents the second face/vector.
// I'm suspecting that a matrix is the way to derive the hash function from the 64 paths to the 
// 24 orientations

struct rotator
{
	int x;
	int y;
	int z;

	rotator()
	{ // load top orientation for default
		x=0;
		y=1;
		z=0;
	}

	rotator(char xi, char yi, char zi)
	{
		x=xi;
		y=yi;
		z=zi;
	}

	rotator(Orientation o)
	{
		loadOrientation(o);
	}

	rotator(byte key)
	{
		// the input is a direction that lines up with Orientation
		Orientation o=(Orientation) key;
		loadOrientation(o);
	}

	void loadOrientation(Orientation o)
	{
		if      (o==top)	{x=0;y=1;z=0;}
		else if (o==bottom)	{x=0;y=-1;z=0;}
		else if (o==left)	{x=-1;y=0;z=0;}
		else if (o==right)	{x=1;y=0;z=0;}
		else if (o==front)	{x=0;y=0;z=1;}
		else if (o==back)	{x=0;y=0;z=-1;}
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

	bool equals(rotator o)
	{
		if ((x == o.x) && (y == o.y) && (z == o.z))
			return true;
		else
			return false;
	}

	bool operator ==(rotator o)
	{
		return equals(o);
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

struct orientVector
{
	rotator tVector; // vector that would be top in the home orientation
	rotator fVector; // vector that would be front in the home orientation

	orientVector()
	{
		tVector=rotator();
		fVector=rotator();
	}

	orientVector(rotator tVectInit, rotator fVectInit)
	{
		tVector = tVectInit;
		fVector = fVectInit;
	}


	bool equals(orientVector o)
	{
		if ((tVector == o.tVector) && (fVector == o.fVector))
			return true;
		else
			return false;
	}

	bool operator ==(orientVector o)
	{
		return equals(o);
	}
};


class Block
{
private:  // make sure the block is created with the proper constructor
	Block(void);

	static bool initialized;
	static rByte normalizer[256];
	static oByte oVector[256];
	static roByte roByteConvert[256];
	void createNormalizers();




public:
	~Block(void);
	Block(Positions home,Faces f);

	Positions homePosition;	
	Faces faces;
	bool equalFaces(Faces f);

	rByte rotate(rByte currentVector, rByte offsetVector);
	rByte getRKey(int xRotations, int yRotations, int zRotations);
	void getRotations(rByte key, int &xRotations, int &yRotations, int &zRotations);
	oByte getOKey(rotator tVector, rotator fVector);
	oByte getOKey(rByte RKey);
	orientVector getOrientation(oByte key);
};


