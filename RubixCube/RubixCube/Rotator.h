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
	
	Rotator is a single vector.  It is the direction that one face is pointing.  You need
	two Rotators to orient a cube.
***********************************************************************************/




class Rotator
{
	Orientation ro;
	static const byte xAxis[4]; 
	static const byte yAxis[4]; 
	static const byte zAxis[4]; 

	static const byte rxAxis[6]; 
	static const byte ryAxis[6]; 
	static const byte rzAxis[6]; 

public:
	Rotator(void);
	~Rotator(void);

	Rotator(Orientation o);
	Rotator(byte key);

	bool equals(Rotator o);
	bool operator ==(Rotator o);
	bool operator !=(Rotator o);

	void incXaxis();
	void incYaxis();
	void incZaxis();
	//void decXaxis();
	//void decYaxis();
	//void decZaxis();

	void loadOrientation(Orientation o);

	Orientation getOrientation();
	Rotator getOpposite();

	static Rotator getAxis(Rotator t,Rotator f);
	static rByte difference(Rotator v1, Rotator v2);

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

	bool isValid()
	{
		// make sure the top and front are at right angles to each other
		if ((tVector==fVector)||(tVector == fVector.getOpposite()))
			return false;
		else
			return true;
	}

	bool equals(orientVector o)
	{
		if ((tVector == o.tVector) && (fVector == o.fVector))
			return true;
		else
			return false;
	}

	static bool isHome(orientVector o)
	{
		if ((o.tVector==Rotator(Orientation::top)) && (o.fVector==Rotator(Orientation::front)))
			return true;
		else 
			return false;
	}

	bool operator ==(orientVector o)
	{
		return equals(o);
	}
};
