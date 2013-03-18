#include "Block.h"

bool Block::initialized = false;
rByte Block::normalizer[256];
oByte Block::oVector[256];
roByte Block::roByteConvert[256];

Block::Block(void)
{
	initialized=true;
	createNormalizers();
}


Block::~Block(void)
{
}

Block::Block(Positions home,Faces f)
{
	initialized=true;
	createNormalizers();
	homePosition=home;
	faces = f;
}

//// returns '-1' if there is no orientation that matches the color pattern.
//int Block::getOrientation(Faces f)
//{
//	// there are 24 orientations so we need to compare each one
//	return -1;
//}


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

****************************************************************************************************/
byte Block::rotate(rByte currentVector, rByte offsetVector)
{
	// Do the rotation
	currentVector += offsetVector;
	// mask any bits that overflowed.
	// mask = 11011011 = 0xDB
	currentVector = (currentVector & 0xDB);
	currentVector = normalizer[currentVector]; // converts the current orientation path to the standard one.
	return currentVector;
}

// this takes a series of rotations and converts it to the key used by normalizer
// and rotate.  It loads the rotations for each direction into a single char
// that can be operated on with standard hardware math functions like "add"
// it is a hash function on the rotation paths
rByte Block::getRKey(int xRotations, int yRotations, int zRotations)
{
	//xRotations leave the same
	yRotations = yRotations << 3;  //shift left 3 bits 
	zRotations = zRotations << 6;  //shift left 6 bits
	return xRotations | yRotations | zRotations; //combine the three together in one byte using bitwise or
}

// reverses getRKey
void Block::getRotations(rByte key, int &xRotations, int &yRotations, int &zRotations)
{
	xRotations = key & 0x03; // 00000011
	yRotations = key & 0x18; // 00011000
	zRotations = key & 0xC0; // 11000000
	yRotations = yRotations >> 3;
	zRotations = zRotations >> 6;
}

// there are only 6 faces or vectors in a cube.  Using the same method as above
// we can create a hash on the two vectors that are used to describe the 24 orientations
oByte Block::getOKey(rotator tVector, rotator fVector)
{
	Orientation top = tVector.getOrientation();
	Orientation front = fVector.getOrientation();
	oByte tKey = (oByte) top;  // converts the orientation enum to a number (0-6)
	oByte fKey = (oByte) front; // converts the orientation enum to a number (0-6)
	// each key takes up three bits since 111 = 7
	fKey=fKey << 3; // shifts the fKey to the left digits
	return tKey | fKey;  // combine the two together in one byte using bitwise or
}

oByte Block::getOKey(rByte RKey)
{
	return roByteConvert[RKey];
}

orientVector Block::getOrientation(oByte key)
{
	oByte tKey = key & 0x07; // 00000111
	oByte fKey = key & 0x38; // 00111000
	fKey = fKey >> 3;
	rotator tVector(tKey);
	rotator fVector(fKey);
	return orientVector(tVector,fVector);
}


// 0 = no rotation 
// 1 = one rotation clockwise 
// 2 = two rotations clockwise (or two rotations counterclockwise)
// 3 = one rotation counterclockwise 

// since order matters, all orientation changes are xAxis then yAxis then zAxis
// This function resolves all orientations and output them to a the normalizer array. This allows for
// a quick way to simplify any rotation path to it's simplest version and also restricts
// the possible rotations to only 24.  It converts the 64 paths into 24 orientations by creating
// a sort of hash.
void Block::createNormalizers()
{
	// clear the normalizer and oVector lists so the empty selection will show
	for (int i=0;i<256;i++)
	{
		normalizer[i]=0xFF;
		oVector[i]=0xFF;
		roByteConvert[i]=0xFF;
	}

	byte xAxis,yAxis,zAxis;
	for (xAxis=0; xAxis <= 3; ++xAxis)
	{
		for (yAxis=0; yAxis <=3; ++yAxis)
		{
			for (zAxis=0; zAxis <=3; ++zAxis)
			{
				// get rotation
				rotator tVector = rotator(0,1,0);  // setup the top rotator
				rotator fVector = rotator(0,0,1);  // setup the front rotator
				for (int xNum=0;xNum<xAxis;++xNum) {tVector.incXaxis();fVector.incXaxis();}
				for (int yNum=0;yNum<yAxis;++yNum) {tVector.incYaxis();fVector.incYaxis();}
				for (int zNum=0;zNum<zAxis;++zNum) {tVector.incZaxis();fVector.incZaxis();}
				
				// get rotation key
				byte rIndex = getRKey(xAxis,yAxis,zAxis);  

				// store all possible rotation paths as both the index and value
				// later we will go through and simplify all of the values to one simplified 
				// key for every orientation. (64 possible orientation paths simplified to 
				// the 24 possible orientations.
				normalizer[rIndex]=rIndex; 

				// get orientation key
				byte oIndex = getOKey(tVector,fVector);
				if (oVector[oIndex]==0xFF)
				{
					// It's empty so load it with the current rIndex
					oVector[oIndex]=rIndex;
				}
				else
				{
					// it's not empty so see if it is smaller or bigger than the previous one
					// calculate the previous rotations
					int tempX,tempY,tempZ;
					getRotations(oVector[oIndex],tempX,tempY,tempZ);
					if ((tempX + tempY + tempZ) <= (xAxis + yAxis + zAxis))
					{
						// the new rotation combination requires fewer rotations than the previous
						// so change to the new version
						oVector[oIndex]=rIndex;
					}
				}
				roByteConvert[rIndex]=oIndex;
			}
		}
	}

	// scan through each normalizer and fill it with the solution that requires the minimum
	// number of rotations.
	for (int i=0;i<256;i++)
	{
		if (normalizer[i]==0xFF) break;
		normalizer[i]=oVector[roByteConvert[i]];  // oVector was already loaded with the minimum value above
	}
}

