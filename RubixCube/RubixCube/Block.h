#pragma once

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

class Faces
{
public:
	Color top, bottom, left, right, front, back;
	Faces copy()
	{
		Faces ret;
		ret.top=top;
		ret.bottom = bottom;
		ret.left = left;
		ret.right = right;
		ret.front = front;
		ret.back = back;
		return ret;
	}

	bool equal(Faces f)
	{
		if ((top == f.top)&&
				(bottom == f.bottom)&&
				(left == f.left)&&
				(right == f.right)&&
				(front == f.front)&&
				(back == f.back))
			return true;
		else
			return false;
	}
};


class Block
{
private:  // make sure the block is created with the proper constructor
	Block(void);

public:
	~Block(void);
	Block(Positions home,Faces f);

	Positions homePosition;	
	Faces faces;
	bool equalFaces(Faces f);


	// returns -1 if there is no orientation that matches the color pattern.
	int getOrientation(Faces f);
	unsigned char rotate(int currentVector, int offsetVector);

};


