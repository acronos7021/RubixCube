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

class Rotator
{
public:
	int x;
	int y;
	int z;

	Rotator(void);
	~Rotator(void);

	Rotator(char xi, char yi, char zi);
	Rotator(Orientation o);
	Rotator(byte key);

	void loadOrientation(Orientation o);
	void incXaxis();
	void incYaxis();
	void incZaxis();
	bool equals(Rotator o);
	bool operator ==(Rotator o);
	Orientation getOrientation();
	Orientation getOrientation(int xi, int yi, int zi);
	std::string toString();

};

struct orientVector
{
	Rotator tVector; // vector that would be top in the home orientation
	Rotator fVector; // vector that would be front in the home orientation

	orientVector()
	{
		tVector=Rotator();
		fVector=Rotator();
	}

	orientVector(Rotator tVectInit, Rotator fVectInit)
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