#pragma once

#include <string>
#include <map>
#include <vector>
#include "Block.h"
#include "Enums.h"

/*********************************************************************
This class is intended to model a Rubix cube.  It keeps track of the current state, and it
allows the cube to be manipulated.  In order to facilitate as robust an AI to solve the
cube as possible, it needs to be as fast as I can possibly make it.  Many design decisions 
are a tradeoff between readability and speed.  In this case I mostly went with speed.

Moves - this is a single rotation of one of the faces of the cube.  It is defined in "Enums.h"

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


class TransferMoves
{
public:
	Positions oldPosition;
	Positions newPosition;
	rByte rotate;

	bool operator==(TransferMoves tm)
	{
		return ((oldPosition==tm.oldPosition) 
			&& (newPosition==tm.newPosition) 
			&& (rotate==tm.rotate));
	}

	friend bool operator< (const TransferMoves& tm1, const TransferMoves& tm2)
	{
		// sort order : oldPosition, newPosition, rotate
		if (tm1.oldPosition==tm2.oldPosition)
		{
			if (tm1.newPosition==tm2.newPosition)
			{
				if (tm1.rotate==tm2.rotate)
					return false;  // tm1 and tm2 are equal
				else
					return tm1.rotate < tm2.rotate;
			}
			else
				return tm1.newPosition < tm2.newPosition;
		}
		else
			return tm1.oldPosition < tm2.oldPosition;
	}

};

bool operator<(const TransferMoves& tm1, const TransferMoves& tm2);



struct TransferFunction
{
	std::vector<TransferMoves> transferBlockMoves;
	std::vector<Moves> transferFaceMoves;
};

class Cube
{
public:

	// constructors and destructors
	Cube(void);
	~Cube(void);
	Cube(Cube* cb);

	// copys the state of the referenced cube into this one.
	void clone(Cube* cb);
	// Executes the specified move/transfer function on this cube.
	void move(int transferFunctionIndex);
	// returns true if this Cube and the refrenced one are the same.
	bool equals(Cube* cb);
	// returns the number of blocks that are in the correct position and orientation
	int unSolved();
	static int Cube::unSolved(Cube c);
	// Adds the specified block to the cube in the specified position.  It checks to make
	// sure no block is added twice and that the block actually exists on a Rubix cube.
	void addBlock(Positions position, Faces f);
	// Checks to make sure this cube has all of the blocks filled in and ready.  If there are any
	// blocks missing, it returns an exception that lists them.
	bool checkCube();

	// scans all of the transfer functions to find the one that brings the cube the
	// closest to being solved.
	int getOptimumTransferFunction();
	// same thing as getOpti... except it goes ahead and makes the move series.
	int doOptimumTransferFunction();

	// move the cube to predefined states
	void SetCubeToSolved();  // fills the cube with all blocks in the home position
	void ClearCube(); // sets the cube to empty or void
	
	// returns the current state of the cube as a string
	std::string toString(bool withBlockNums);

	void calculateAdvancedTransferFunctions();
private:
	TransferMoves createTransferMove(Positions oldPosition, Positions newPosition, rByte rotate);

//	static TransferMoves* transferBlockMoves; //lists how the individual blocks move in a transfer
//	static char* transferFaceMoves; //lists the face Moves that would be required to do the transfer
//	static TransferFunctions* transferFunctions; // index for the previous two lists
	static std::vector<TransferFunction> transferFunctions;
	static std::map<std::vector<TransferMoves>,int> reverseTransferFunctionMap;  //,std::hash<int>
	typedef std::pair<const std::vector<TransferMoves>,int> reverseTransferFunctionPair;

	// Stores the data on the 20 blocks that make up the moving portion of the cube.  This is not
	// the current state of the cube, but rather the building blocks needed to portray the current state.
	static BasicBlock homeBlocks[20];
	Block blocks2[20];

	// These are initialized once and shared among all of the instances of Cube trough the above
	// static variables.

	// initialization
	static bool initialized; // ensures everything is only initialized once
	void initHomeBlocks();
	void initBasicTransferFunctions();

	std::vector<TransferMoves> calculateTransferMove(Cube oldCube,Cube newCube);
	bool isSameTransferMoves(std::vector<TransferMoves> mv1, std::vector<TransferMoves> mv2);

	// used by toString
	std::string getBlockFaceStr(Positions p,Orientation o,bool withBlockNum);
	std::string convertColorToLetter(Color c);
};