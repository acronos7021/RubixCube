#pragma once

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

