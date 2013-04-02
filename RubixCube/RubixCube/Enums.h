#pragma once
#include <unordered_map>

typedef unsigned char byte;
typedef unsigned char oByte;
typedef unsigned char rByte;
//typedef unsigned char roByte;

// describes the direction the block is facing from it's home position
enum class Orientation : unsigned char {top,bottom,left,right,front,back,invalid};

// the color for each face in the home position respectively
enum class Color : unsigned char {blue,green,orange,red,white,yellow,none};

// the possible positions of the blocks.  Organized as [height][width][depth]
enum class Positions : unsigned char {topLeftFront,topLeftMiddle,topLeftBack,
				topMiddleFront,topMiddleBack,
				topRightFront,topRightMiddle,topRightBack,				
				middleLeftFront,middleLeftBack,middleRightFront,middleRightBack,
				bottomLeftFront,bottomLeftMiddle,bottomLeftBack,
				bottomMiddleFront, bottomMiddleBack,
				bottomRightFront,bottomRightMiddle,bottomRightBack,
				noPositionMatches};

// the basic moves for the faces of the cube
enum class Moves : unsigned char {topClockwise,topCounterClockwise,
									bottomClockwise,bottomCounterClockwise,
									rightClockwise,rightCounterClockwise,
									leftClockwise,leftCounterClockwise,
									frontClockwise,frontCounterClockwise,
									backClockwise,backCounterClockwise};


// stores the colors for each of the faces.  Orientation is handled by other code.
class Faces
{
public:
	Color topColor, bottomColor, leftColor, rightColor, frontColor, backColor;
	Faces() {};
	Faces(Color to, Color bo, Color le, Color ri, Color fr, Color ba)
	{
		topColor=to;
		bottomColor = bo;
		leftColor = le;
		rightColor = ri;
		frontColor = fr;
		backColor = ba;
	}

	Faces copy()
	{
		Faces ret;
		ret.topColor=topColor;
		ret.bottomColor = bottomColor;
		ret.leftColor = leftColor;
		ret.rightColor = rightColor;
		ret.frontColor = frontColor;
		ret.backColor = backColor;
		return ret;
	}

	bool equal(Faces f)
	{
		if ((topColor == f.topColor)&&
				(bottomColor == f.bottomColor)&&
				(leftColor == f.leftColor)&&
				(rightColor == f.rightColor)&&
				(frontColor == f.frontColor)&&
				(backColor == f.backColor))
			return true;
		else
			return false;
	}

	bool operator ==(Faces f)
	{
		return equal(f);
	}

};