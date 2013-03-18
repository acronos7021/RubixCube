#pragma once

typedef unsigned char byte;
typedef unsigned char oByte;
typedef unsigned char rByte;
typedef unsigned char roByte;

// describes the direction the block is facing from it's home position
enum Orientation {top,bottom,left,right,front,back,invalid};

// the color for each face in the home position respectively
enum Color {blue,green,orange,red,white,yellow,none};

// the possible positions of the blocks.  Organized as [height][width][depth]
enum Positions {topLeftFront,topLeftMiddle,topLeftBack,
				topMiddleFront,topMiddleBack,
				topRightFront,topRightMiddle,topRightBack,				
				middleLeftFront,middleLeftBack,middleRightFront,middleRightBack,
				bottomLeftFront,bottomLeftMiddle,bottomLeftBack,
				bottomMiddleFront, bottomMiddleBack,
				bottomRightMiddle,bottomRightFront,bottomRightBack};

// the basic moves for the faces of the cube
enum Moves {topClockwise,topCounterClockwise,
			bottomClockwise,bottomCounterClockwise,
			rightClockwise,rightCounterClockwise,
			leftClockwise,leftCounterClockwise,
			frontClockwise,frontCounterClockwise,
			backClockwise,backCounterClockwise};

class Faces
{
public:
	Color top, bottom, left, right, front, back;
	Faces() {};
	Faces(Color to, Color bo, Color le, Color ri, Color fr, Color ba)
	{
		top=to;
		bottom = bo;
		left = le;
		right = ri;
		front = fr;
		back = ba;
	}

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