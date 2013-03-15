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

// Adds the specified block to the cube in the specified position
void Cube::addBlock(Positions position, Color top, Color bottom, Color left, Color right, 
	Color front, Color back)
{
	// Check if the block has already been added
	
}
