#include "Block.h"


Block::Block(void)
{
}


Block::~Block(void)
{
}

Block::Block(Positions home,Faces f)
{
	homePosition=home;
	faces = f;
}

// returns '-1' if there is no orientation that matches the color pattern.
int Block::getOrientation(Faces f)
{
	// there are 24 orientations so we need to compare each one
	return -1;
}


/****************************************************************************************************
	There are 24 orientations.  The orientations are represented by 3 sets of 3 bits.
	The active bits are [0]11 011 011 in a byte.  The extra zero is to leave space for the bits
	to overflow when they are added together.  For instance, 011 + 011 = 110.  If we
	drop the top 1 we get the new rotation position on that axis as 10.  What this means
	is that if you take	a cube that has already been rotated 3 times and you rotate it 
	3 more on the same axis you get a rotation that is 2 from home on that axis.  This matches the
	binary representation and solution described above.

	Use an 'add' and a bitwise 'and' to mask the overflow bits on b3,b6,carry.  
	Think about this as 3 variables, each can rotate from 0 to 3.  Once one of 
	the variables gets to 4 we change it back to 0.  Each variable is represented
	by 2 bits on an 8 bit byte.  There is a gap of one bit between each set of
	two bits to allow that variable to overflow back to 0 when it becomes 4.

	All rotations will have to be positive.  For a negative rotation, add 3 on that axis. 

	*** i'm not sure this will work yet.  It actually results in 64 possible orientations which
	is wrong ***
****************************************************************************************************/
unsigned char Block::rotate(int currentVector, int offsetVector)
{
	// Do the rotation
	currentVector += offsetVector;
	// mask any bits that overflowed.
	// mask = 11011011 = 0xDB
	currentVector = (currentVector & 0xDB);
	return currentVector;
}
