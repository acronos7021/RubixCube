#include "Cube.h"

// makes the static variable "initalized" be set to false
// the first time Cube is instantiated.
bool Cube::initialized = false;

Cube::Cube(void)
{
	if (!initialized)
	{
		initBlocks();
		initTransferFunctions();
		initialized = true;
	}
}

Cube::~Cube(void)
{
}

void Cube::initBlocks()
{
}
	
void Cube::initTransferFunctions()
{
}

// Adds the specified block to the cube in the specified position
void Cube::addBlock(Positions position, Color top, Color bottom, Color left, Color right, 
	Color front, Color back)
{
	// Check if the block has already been added
	
}

// copys the state of the referenced cube into this one.
void Cube::clone(Cube* c)
{
	for (int i=0;i<20;++i)
	{
		blockStatus[i] = c->blockStatus[i];
	}
}

// returns true if this Cube and the refrenced one are the same.
bool Cube::equals(Cube* c)
{
	for (int i=0;i<20;++i)
	{
		if (blockStatus[i].equals(c->blockStatus[i])) return false;
	}
	// if we got here then all of the blocks are the same.
	return true;
}

// Executes the specified move/transfer function on this cube.
void Cube::move(Moves mv)
{
}


// returns the number of blocks that are in the correct position and orientation
int Cube::solved()
{
	return 0;
}


// Checks to make sure this cube has all of the blocks filled in and ready.  If there are any
// blocks missing, it returns an exception that lists them.
bool Cube::checkCube()
{
	return false;
}

// scans all of the transfer functions to find the one that brings the cube the
// closest to being solved.
int Cube::getOptimumTransferFunction()
{
	return 0;
}

// same thing as getOptimumTransferFunction except it goes ahead and makes the move series.
int Cube::doOptimumTransferFunction()
{
	return 0;
}



