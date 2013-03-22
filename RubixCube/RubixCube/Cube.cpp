#include "Cube.h"

// makes the static variable "initalized" be set to false
// the first time Cube is instantiated.
bool Cube::initialized = false;
Block Cube::blocks[20];

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

// the possible positions of the blocks.  Organized as [height][width][depth]
void Cube::initBlocks()
{
	// there are 20 blocks.
	// the orientations are set to home by default
	//										top,bottom,left,right,front,back
	blocks[0].initBlock(topLeftFront,Faces(blue,none,orange,none,white,none));
	blocks[1].initBlock(topLeftMiddle,Faces(red,blue,green,none,none,none));
	blocks[2].initBlock(topLeftBack,Faces(red,blue,green,none,none,none));
	blocks[3].initBlock(topMiddleFront,Faces(red,blue,green,none,none,none));
	blocks[4].initBlock(topMiddleBack,Faces(red,blue,green,none,none,none));
	blocks[5].initBlock(topRightFront,Faces(red,blue,green,none,none,none));
	blocks[6].initBlock(topRightMiddle,Faces(red,blue,green,none,none,none));
	blocks[7].initBlock(topRightBack,Faces(red,blue,green,none,none,none));
	blocks[8].initBlock(middleLeftFront,Faces(red,blue,green,none,none,none));
	blocks[9].initBlock(middleLeftBack,Faces(red,blue,green,none,none,none));
	blocks[10].initBlock(middleRightFront,Faces(red,blue,green,none,none,none));
	blocks[11].initBlock(middleRightBack,Faces(red,blue,green,none,none,none));
	blocks[12].initBlock(bottomLeftFront,Faces(red,blue,green,none,none,none));
	blocks[13].initBlock(bottomLeftMiddle,Faces(red,blue,green,none,none,none));
	blocks[14].initBlock(bottomLeftBack,Faces(red,blue,green,none,none,none));
	blocks[15].initBlock(bottomMiddleFront,Faces(red,blue,green,none,none,none));
	blocks[16].initBlock(bottomMiddleBack,Faces(red,blue,green,none,none,none));
	blocks[17].initBlock(bottomRightMiddle,Faces(red,blue,green,none,none,none));
	blocks[18].initBlock(bottomRightFront,Faces(red,blue,green,none,none,none));
	blocks[19].initBlock(bottomRightBack,Faces(red,blue,green,none,none,none));
}
	
void Cube::initTransferFunctions()
{
}

// Adds the specified block to the cube in the specified position
void Cube::addBlock(Positions position, Color topColor, Color bottomColor, Color leftColor, 
	Color rightColor, Color frontColor, Color backColor)
{
	// Check if the block has already been added
	
}

// copys the state of the referenced cube into this one.
void Cube::clone(Cube* c)
{
	for (int i=0;i<20;++i)
	{
		blocks[i] = c->blocks[i];
	}
}

// returns true if this Cube and the refrenced one are the same.
bool Cube::equals(Cube* c)
{
	for (int i=0;i<20;++i)
	{
		if (blocks[i].equalOrientation(c->blocks[i])) return false;
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
	int solvedBlocks=0;
	for (int i=0;i<20;++i)
	{
		if (blocks[i].isHome()) 
			solvedBlocks++;
	}
	return solvedBlocks;
}


// Checks to make sure this cube has all of the blocks filled in and ready.  If there are any
// blocks missing, it returns an exception that lists them.
bool Cube::checkCube()
{
	int loadedCubes=0;
	for (int i=0;i<20;++i)
		if (blocks[i].loaded)
			loadedCubes++;
	if (loadedCubes==20)
		return true;
	else 
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



