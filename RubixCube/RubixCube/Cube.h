#pragma once

#include "Block.h"

/*********************************************************************
This class is intended to model a Rubix cube.  It keeps track of the current state, and it
allows the cube to be manipulated.  In order to facilitate as robust an AI to solve the
cube as possible, it needs to be as fast as I can possibly make it.  Many design decisions 
are a tradeoff between readability and speed.  In this case I mostly went with speed.

Moves - this is a single rotation of one of the faces of the cube.

Orientation - Each block has a home position which is how it would sit in a solved cube.  The orientation
is the direction the top of the block would be facing if it were in it's home position.  So a block in
'top' orientation is in it's home position.  In any other orientation the block has been rotated.

TransferFunctions are used to change the cube from one state to another.  They include
the movements of the individual blocks for a series of Moves.  This includes the series of the basic
single moves of each face.  Doing this allows many Moves to be emulated in a single Transfer.
A total of 8 blocks are moved in a basic face rotation.  However, if your combine 2 or more face rotations,
you can never mover more than the 20 blocks and it is possible to move as few as 2 or 3 in
a more advanced series.  If you were to try to do a 5 face Moves series, you would have to move
8 x 5 = 40 blocks to do it linearly and do so in a loop.  It will be dramatically faster to make the 
transfer involved in a 5 move series in one smooth and fast function.  The tradeoff is that this requires 
all of the transfer functions to be calculated ahead of time.

*********************************************************************/

// describes the direction the block is facing from it's home position
enum Orientation {top,bottom,left,right,front,back};
// the color for each face in the home position respectively
enum Color {blue,green,orange,red,white,yellow};
// the possible positions of the blocks.  Organized as [height][width][depth]
enum Positions {topLeftFront,topLeftMiddle,topLeftBack,
				topMiddleFront,topMiddleBack,
				topRightBack,topRightMiddle,topRightFront,				
				middleLeftFront,middleLeftBack,middleRightBack,middleRightFront,
				bottomLeftFront,bottomLeftMiddle,bottomLeftBack,
				bottomMiddleFront, bottomMiddleBack,
				bottomRightBack,bottomRightMiddle,bottomRightFront};

class Cube
{
public:

	// constructors and destructors
	Cube(void);
	~Cube(void);
	Cube(Cube* cb);

	// the basic moves for the faces of the cube
	enum Moves {topClockwise,topCounterClockwise,
			   bottomClockwise,bottomCounterClockwise,
			   rightClockwise,rightCounterClockwise,
			   leftClockwise,leftCounterClockwise,
			   frontClockwise,frontCounterClockwise,
			   backClockwise,backCounterClockwise};



	// copys the state of the referenced cube into this one.
	void clone(Cube* cb);
	// Executes the specified move on this cube.
	void move(Moves mv);
	// returns true if this Cube and the refrenced one are the same.
	bool equals(Cube* cb);
	// returns the number of blocks that are in the correct position and orientation
	int solved();
	// Adds the specified block to the cube in the specified position
	void addBlock(Color top, Color bottom, Color left, Color right, Color front, Color back);
	// Checks to make sure this cube has all of the blocks filled in and ready.  Returns an
	// exception that lists any blocks that are missing if there are.
	bool checkCube();

	// scans all of the transfer functions to find the one that brings the cube the
	// closest to being solved.
	int getOptimumTransferFunction();
	// same thing as getOpti... except it goes ahead and makes the move series.
	int doOptimumTransferFunction();

private:
	// One of the many tradeoffs for speed.  A fixed element size and unchanging array size is faster
	// than one that has a variable list of moves attached to it.  By moving the list of moves
	// to another fixed size array, I gain some speed.  Loading this series once
	// when the first cube is instantiated allows very fast access to the transfer functions.
	struct TransferFunctions
	{
		int start,end;  // stores the start and end locations in the transfer array for this function
	};
	// These are initialized once and shared among all of the instances of Cube
	static char* transferMoves;
	static TransferFunctions* transferFunctions;
	static Block* blocks;

	void initTransferFunctions();
	void initBlocks();

};