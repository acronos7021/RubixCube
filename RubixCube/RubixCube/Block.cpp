#include "Block.h"


bool BasicBlock::initialized = false;
rByte BasicBlock::normalizer[256];
rByte BasicBlock::oVector[256];
oByte BasicBlock::roByteConvert[256];

BasicBlock::BasicBlock(void)
{
	if (!initialized)
		createNormalizers();
	initialized=true;
	//loaded=false;
}


BasicBlock::~BasicBlock(void)
{
}

BasicBlock::BasicBlock( Positions home,Faces f)
{
	initialized=true;
	createNormalizers();
	initBlock(home,f);
	//loaded=false;
}

void BasicBlock::initBlock(Positions home,Faces f)
{
	homePosition=home;
	homeOrientation=orientVector(Orientation::top,Orientation::front);
	faces = f;
}

// returns true and assigns that orientation if the there is an orientation that matches this face

orientVector BasicBlock::checkBlock(Faces f)
{
	// check if this face combination fits this block

	// scan through each each orientation of this block to see if it matches
	for (int i=0;i<=83;++i) // 83 is the highest oVector that is actually used
	{
		if (oVector[i]!=255)
		{
			// this vector is in use
			orientVector o = getOrientation(oVector[i]);
			Faces rColors = remapFaces(o); // gets the orientVector
			if (rColors == faces)
			{
				// an orientation was found that matched this block
				// so load the block
				return o;
			}
		}
	}
	return orientVector(Rotator(Orientation::top),Rotator(Orientation::top)); // return an invalid orientation
}

Block BasicBlock::loadBlock(byte homeBlock,Positions p,orientVector o)
{
	//loaded=true;
	Block b;
	b.homeBlock=homeBlock;
	b.orientationVector=o;
	b.position=p;
	return b;
}

// used by remapFaces to put each face color in place.
Faces BasicBlock::addFaceColor(Faces &f, Orientation o, Color c)
{
	if (o == Orientation::top) f.topColor=c;
	if (o == Orientation::bottom) f.bottomColor=c;
	if (o == Orientation::left) f.leftColor=c;
	if (o == Orientation::right) f.rightColor=c;
	if (o == Orientation::front) f.frontColor=c;
	if (o == Orientation::back) f.backColor=c;
	return f;
}

// takes the colors of this block and rotates the block according to the selected vector  It
// then returns the face colors as if they were in that position.
// This doesn't actually rotate the block, it just rotates some vectors that match the block
Faces BasicBlock::remapFaces(orientVector selVect)
{
	// there is probably a better way to do this, but for now I just want to get it done.

	// map home vectors and assign the color to each face
	// then rotate the vectors to the correct position
	Rotator to = Rotator(Orientation::top);
	Rotator bo = Rotator(Orientation::bottom);
	Rotator ri = Rotator(Orientation::right);
	Rotator le = Rotator(Orientation::left);
	Rotator fr = Rotator(Orientation::front);
	Rotator ba = Rotator(Orientation::back);

	// rotate these vectors by the correct amount
	int xRotations,yRotations,zRotations;
	rByte rKey = oVector[getOKey(selVect.tVector,selVect.fVector)]; // convert the vector to a rotation
	getRotations(rKey,xRotations,yRotations,zRotations);
	// do rotations
	for (;xRotations>0;xRotations--)
	{
		to.incXaxis();
		bo.incXaxis();
		ri.incXaxis();
		le.incXaxis();
		fr.incXaxis();
		ba.incXaxis();
	}
	for (;yRotations>0;yRotations--)
	{
		to.incYaxis();
		bo.incYaxis();
		ri.incYaxis();
		le.incYaxis();
		fr.incYaxis();
		ba.incYaxis();
	}
	for (;zRotations>0;zRotations--)
	{
		to.incZaxis();
		bo.incZaxis();
		ri.incZaxis();
		le.incZaxis();
		fr.incZaxis();
		ba.incZaxis();
	}
	// add the face colors in the correct locations
	Faces f;
	addFaceColor(f,to.getOrientation(),faces.topColor);
	addFaceColor(f,bo.getOrientation(),faces.bottomColor);
	addFaceColor(f,ri.getOrientation(),faces.rightColor);
	addFaceColor(f,le.getOrientation(),faces.leftColor);
	addFaceColor(f,fr.getOrientation(),faces.frontColor);
	addFaceColor(f,ba.getOrientation(),faces.backColor);
	
	return f;
}


bool BasicBlock::equalOrientation(Block b1, Block b2)
{
	return b1.orientationVector==b2.orientationVector;
}

bool BasicBlock::isHome(Block b)
{
	if ((b.homeBlock==(byte) homePosition) && (b.position == homePosition) && (b.orientationVector == homeOrientation))
		return true;
	else
		return false;
}

Positions BasicBlock::getHome()
{
	return homePosition;
}

Faces BasicBlock::getFaces(orientVector o)
{
	return faces.copy();
}

bool BasicBlock::equalBlocks(Block b1, Block b2)
{
	if (b1.homeBlock != b2.homeBlock) return false;
	if (!b1.orientationVector.equals(b2.orientationVector)) return false;
	if (b1.position != b2.position) return false;
	// if we got here, they should be the same
	return true;
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

****************************************************************************************************/
//byte Block::rotate(rByte currentVector, rByte offsetVector)
//{
//	// Do the rotation
//	currentVector += offsetVector;
//	// mask any bits that overflowed.
//	// mask = 11011011 = 0xDB 
//	currentVector = (currentVector & 0xDB);
//	currentVector = normalizer[currentVector]; // converts the current orientation path to the standard one.
//	return currentVector;
//}

void BasicBlock::rotate(Block &block, rByte offsetVector)
{
	int xRotations = (offsetVector >> 6) & 0x03; 
	int yRotations = (offsetVector >> 3) & 0x03;
	int zRotations = offsetVector & 0x03;
	for (;xRotations>0;xRotations--) { block.orientationVector.fVector.incXaxis(); block.orientationVector.tVector.incXaxis();}
	for (;yRotations>0;yRotations--) { block.orientationVector.fVector.incYaxis(); block.orientationVector.tVector.incYaxis();}
	for (;zRotations>0;zRotations--) { block.orientationVector.fVector.incZaxis(); block.orientationVector.tVector.incZaxis();}
}

// This function does not actually rotate the block.  It just calculates the difference
// between the current rotation and an old rotation and returns the moves that would have
// been required to move between them.  It is called each time a new transfer function is
// discovered to calculate what transfers to add.
//rByte BasicBlock::deRotate(orientVector oldRotationVector, orientVector newRotationVector)
//{
//	// rotate both orientVectors to to oldRotationVector home
//	// copy currentOrienation vector so that our changes don't affect this block orientation
//	//orientVector newRotationVector = currentOrientation;
//	// get rotation numbers needed to create the current rotation
//	// The oByte can be converted to the rByte which can be converted to the correct orientations
//	oByte currOByte = getOKey(newRotationVector.tVector,newRotationVector.fVector);
//	// convert oByte to rByte
//	rByte currRByte = oVector[currOByte];
//	// calculate rotations
//	int xRotations = (currRByte >> 6) & 0x03;
//	int yRotations = (currRByte >> 3) & 0x03;
//	int zRotations = currRByte & 0x03;
//	// do rotations in reverse order
//	for (;zRotations>0;zRotations--) 
//	{ 
//		newRotationVector.fVector.decZaxis(); newRotationVector.tVector.decZaxis();
//		oldRotationVector.fVector.decZaxis(); oldRotationVector.tVector.decZaxis();
//	}
//	for (;yRotations>0;yRotations--)
//	{ 
//		newRotationVector.fVector.decYaxis(); newRotationVector.tVector.decYaxis();
//		oldRotationVector.fVector.decYaxis(); oldRotationVector.tVector.decYaxis();
//	}
//	for (;xRotations>0;xRotations--)
//	{ 
//		newRotationVector.fVector.decXaxis(); newRotationVector.tVector.decXaxis();
//		oldRotationVector.fVector.decXaxis(); oldRotationVector.tVector.decXaxis();
//	}
//	// if it worked correctly, newRotationVector should be in it's home position.
//	// now the oldRotationVector rotation should be where it would be oriented 
//	// if it had started from home position. Now our normalizers should work directly on it.
//	oByte oldObyte = getOKey(oldRotationVector.tVector,oldRotationVector.fVector);
//	return oVector[oldObyte];  // return the rByte for the difference.
//}


// deRotate is always clockwise.  It takes the two vectors and calculates the clockwise
// rotations that would be required to move from one to the other.
rByte BasicBlock::deRotate(orientVector oldRotationVector, orientVector newRotationVector)
{
	rByte newPlane = Rotator::difference(newRotationVector.tVector,newRotationVector.fVector);
	rByte oldPlane = Rotator::difference(oldRotationVector.tVector,oldRotationVector.fVector);
	rByte tDiff = Rotator::difference(newRotationVector.tVector,oldRotationVector.tVector);
	rByte fDiff = Rotator::difference(newRotationVector.fVector,newRotationVector.fVector);

	// if either of them rotates in a directions, then that direction is important
	// so, combine them.
	return tDiff | fDiff;

	////Rotator newRotator;
	////Rotator oldRotator;
	//if (oldRotationVector.tVector==newRotationVector.tVector)
	//{
	//	// it must either be fVector that is rotating or there is no rotation, tVector is the axis
	//	return normalizer[Rotator::difference(newRotationVector.fVector,oldRotationVector.fVector)];
	//}
	//else
	//{
	//	// either tVector is doing the rotation or both of them are rotating the same way
	//	if (oldRotationVector.fVector==newRotationVector.fVector)
	//	{
	//		// tVector is doing the rotation. fVector is the axis
	//		return normalizer[Rotator::difference(newRotationVector.tVector,oldRotationVector.tVector)];
	//	}
	//	else
	//	{
	//		// both vectors are doing the rotation so we need to calculate the axis
	//		Rotator axis = Rotator::getAxis(newRotationVector.tVector,newRotationVector.fVector);
	//		return normalizer[Rotator::difference(newRotationVector.tVector,oldRotationVector.tVector)];
	//	}
	//}
}


	//oByte newOByte = getOKey(newRotationVector.tVector,newRotationVector.fVector);
	//// convert oByte to rByte
	//rByte newRByte = oVector[newOByte];
	//// calculate rotations
	//int newXrotations = (newRByte >> 6) & 0x03;
	//int newYrotations = (newRByte >> 3) & 0x03;
	//int newZrotations = newRByte & 0x03;

	//oByte oldOByte = getOKey(oldRotationVector.tVector,oldRotationVector.fVector);
	//// convert oByte to rByte
	//rByte oldRByte = oVector[oldOByte];
	//// calculate rotations
	//int oldXrotations = (oldRByte >> 6) & 0x03;
	//int oldYrotations = (oldRByte >> 3) & 0x03;
	//int oldZrotations = oldRByte & 0x03;

	//int diffXrotations = (oldXrotations + 4 - newXrotations) % 4;
	//int diffYrotations = (oldYrotations + 4 - newYrotations) % 4;
	//int diffZrotations = (oldZrotations + 4 - newZrotations) % 4;

	//rByte diffRbyte = getNRKey(diffXrotations,diffYrotations,diffZrotations);
	//return diffRbyte;
//}

// this takes a series of rotations and converts it to the key used by normalizer
// and rotate.  It loads the rotations for each direction into a single char
// that can be operated on with standard hardware math functions like "add"
// it is a hash function on the rotation paths
rByte BasicBlock::getRKey(int xRotations, int yRotations, int zRotations)
{
	//xRotations leave the same
	xRotations = xRotations << 6;  //shift left 6 bits
	yRotations = yRotations << 3;  //shift left 3 bits 
	rByte rKey = xRotations | yRotations | zRotations;
	return rKey; //combine the three together in one byte using bitwise or
}

// the same as getRKey but it normalizes the result
rByte BasicBlock::getNRKey(int xRotations,int yRotations, int zRotations)
{
	xRotations = xRotations % 4;
	yRotations = yRotations % 4;
	zRotations = zRotations % 4;
	//xRotations leave the same
	xRotations = xRotations << 6;  //shift left 6 bits
	yRotations = yRotations << 3;  //shift left 3 bits 
	rByte rKey = xRotations | yRotations | zRotations;
	rKey = normalizer[rKey];
	return rKey; //combine the three together in one byte using bitwise or
}

// reverses getRKey
void BasicBlock::getRotations(rByte rKey, int &xRotations, int &yRotations, int &zRotations)
{
	xRotations = rKey & 0xC0; // 11000000
	yRotations = rKey & 0x18; // 00011000
	zRotations = rKey & 0x03; // 00000011
	xRotations = xRotations >> 6;
	yRotations = yRotations >> 3;
}

// there are only 6 faces or vectors in a cube.  Using the same method as above
// we can create a hash on the two vectors that are used to describe the 24 orientations
oByte BasicBlock::getOKey(Rotator tVector, Rotator fVector)
{
	Orientation oTop = tVector.getOrientation();
	Orientation oFront = fVector.getOrientation();
	oByte tKey = (oByte) oTop;  // converts the orientation enum to a number (0-6)
	oByte fKey = (oByte) oFront; // converts the orientation enum to a number (0-6)
	// each key takes up three bits since 111 = 7
	tKey=tKey << 4; // shifts the fKey to the left digits
	return tKey | fKey;  // combine the two together in one byte using bitwise or
}

oByte BasicBlock::getOKey(rByte rKey)
{
	return roByteConvert[rKey];
}

orientVector BasicBlock::getOrientation(oByte oKey)
{
	oByte tKey = oKey & 0xF0; // 11110000
	oByte fKey = oKey & 0x0F; // 00001111
	tKey = tKey >> 4;
	Rotator tVector(tKey);
	Rotator fVector(fKey);
	return orientVector(tVector,fVector);
}


// 0 = no rotation 
// 1 = one rotation clockwise 
// 2 = two rotations clockwise (or two rotations counterclockwise)
// 3 = one rotation counterclockwise 

// since order matters, all orientation changes are xAxis then yAxis then zAxis
// This function resolves all orientations and outputs them to a normalizer array. This allows for
// a quick way to simplify any rotation path to it's simplest version and also restricts
// the possible rotations to only 24.  It converts the 64 paths into 24 orientations by creating
// a sort of hash.
void BasicBlock::createNormalizers()
{
	// clear the normalizer and oVector lists so the empty selection will show
	for (int i=0;i<256;i++)
	{
		normalizer[i]=0xFF;
		oVector[i]=0xFF;
		roByteConvert[i]=0xFF;
	}

	// used for debugging and understanding what is going on.
	int normalizerCount=0;
	int oVectorCount=0;
	int oVectorChangeCount=0;

	byte xAxis,yAxis,zAxis;
	for (xAxis=0; xAxis <= 3; ++xAxis)
	{
		for (yAxis=0; yAxis <=3; ++yAxis)
		{
			for (zAxis=0; zAxis <=3; ++zAxis)
			{
				// get rotation
				Rotator tVector = Rotator(Orientation::top);  // setup the top rotator
				Rotator fVector = Rotator(Orientation::front);  // setup the front rotator
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
				normalizerCount++;

				// get orientation key
				byte oIndex = getOKey(tVector,fVector);
				if (oVector[oIndex]==0xFF)
				{
					// It's empty so load it with the current rIndex
					oVector[oIndex]=rIndex;
					oVectorCount++;
				}
				else
				{
					// it's not empty so see if it is smaller or bigger than the previous one
					// calculate the previous rotations
					int tempX,tempY,tempZ;
					getRotations(oVector[oIndex],tempX,tempY,tempZ);
					if ((xAxis + yAxis + zAxis) < (tempX + tempY + tempZ))
					{
						// paraphrased
						// if ((current rotations needed) < (old rotations needed))
						//		update oVector with the new improved version
						// the goal is to only store the best 24 rotation combinations everything
						// else should come back to these 24
						oVector[oIndex]=rIndex;
						oVectorChangeCount++;
					}
				}
				roByteConvert[rIndex]=oIndex;
			}
		}
	}

	int normalizerChanges=0;
	// scan through each normalizer and fill it with the solution that requires the minimum
	// number of rotations.  There should be 40 adjustments since 64-24=40
	for (int i=0;i<256;i++)
	{
		if (normalizer[i]!=0xFF)  // if it's 0xFF it is not used
		{
			if (normalizer[i] != oVector[roByteConvert[i]])
			{
				normalizer[i]=oVector[roByteConvert[i]];  // oVector was already loaded with the minimum value above
				normalizerChanges++;
			}
		}
	}
}

