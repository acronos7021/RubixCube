#include <sstream>
#include <vector>
#include "Cube.h"

// makes the static variable "initalized" be set to false
// the first time Cube is instantiated.
bool Cube::initialized = false;
BasicBlock Cube::homeBlocks[20];

Cube::Cube(void)
{
	if (!initialized)
	{
		initHomeBlocks();
		initTransferFunctions();
		initialized = true;
	}
	ClearCube();
}

Cube::Cube(Cube* cb)
{
	clone(cb);
}

Cube::~Cube(void)
{

}

void Cube::SetCubeToSolved()
{
	// cube defaults to the home position
	// 0-20 are the 20 positions enumberated by "Positions"
	for (int i=0;i<20;i++)
	{
		blocks2[i].homeBlock= i;
		blocks2[i].position = homeBlocks[i].homePosition;
		blocks2[i].orientationVector = homeBlocks[i].homeOrientation;
	}
}

void Cube::ClearCube()
{
	// empty all of the blocks
	// 0-20 are the 20 positions enumberated by "Positions"
	for (int i=0;i<20;i++)
	{
		blocks2[i].homeBlock = 0xff;
		blocks2[i].position = noPositionMatches;
		blocks2[i].orientationVector = orientVector(Rotator(top),Rotator(top));
	}
}

// the possible positions of the blocks.  Organized as [height][width][depth]
void Cube::initHomeBlocks()
{
	// there are 20 blocks.
	//										the orientations are set to home by default
									//		 	     top,bottom,left,right,front,back
									//			     blue,green,orange,red,white,yellow
	homeBlocks[0].initBlock(topLeftFront,		Faces(blue,none,orange,none,white,none));
	homeBlocks[1].initBlock(topLeftMiddle,		Faces(blue,none,orange,none,none,none));
	homeBlocks[2].initBlock(topLeftBack,		Faces(blue,none,orange,none,none,yellow));
	homeBlocks[3].initBlock(topMiddleFront,		Faces(blue,none,none,none,white,none));
	homeBlocks[4].initBlock(topMiddleBack,		Faces(blue,none,none,none,none,yellow));
	homeBlocks[5].initBlock(topRightFront,		Faces(blue,none,none,red,white,none));
	homeBlocks[6].initBlock(topRightMiddle,		Faces(blue,none,none,red,none,none));
	homeBlocks[7].initBlock(topRightBack,		Faces(blue,none,none,red,none,yellow));
	homeBlocks[8].initBlock(middleLeftFront,	Faces(none,none,orange,none,white,none));
	homeBlocks[9].initBlock(middleLeftBack,		Faces(none,none,orange,none,none,yellow));
	homeBlocks[10].initBlock(middleRightFront,	Faces(none,none,none,red,white,none));
	homeBlocks[11].initBlock(middleRightBack,	Faces(none,none,none,red,none,yellow));
	homeBlocks[12].initBlock(bottomLeftFront,	Faces(none,green,orange,none,white,none));
	homeBlocks[13].initBlock(bottomLeftMiddle,	Faces(none,green,orange,none,none,none));
	homeBlocks[14].initBlock(bottomLeftBack,	Faces(none,green,orange,none,none,yellow));
	homeBlocks[15].initBlock(bottomMiddleFront,	Faces(none,green,none,none,white,none));
	homeBlocks[16].initBlock(bottomMiddleBack,	Faces(none,green,none,none,none,yellow));
	homeBlocks[17].initBlock(bottomRightMiddle,	Faces(none,green,none,red,white,none));
	homeBlocks[18].initBlock(bottomRightFront,	Faces(none,green,none,red,none,none));
	homeBlocks[19].initBlock(bottomRightBack,	Faces(none,green,none,red,none,yellow));
}
	
void Cube::initTransferFunctions()
{
	// the first 12 transfer functions are the basic moves on each face
	// each block has to lead to the next without deleting the previous
	// for now we will use a vector because its easier.  Optomize later
	
	// topClockwisetop
	TransferFunction tcw;
	tcw.transferFaceMoves.push_back(topClockwise);
	rByte key = BasicBlock::getRKey(0,1,0);
	tcw.transferBlockMoves.push_back(createTransferMove(topLeftFront,topLeftBack,key));
	tcw.transferBlockMoves.push_back(createTransferMove(topLeftBack,topRightBack,key));
	tcw.transferBlockMoves.push_back(createTransferMove(topRightBack,topRightFront,key));
	tcw.transferBlockMoves.push_back(createTransferMove(topRightFront,topLeftFront,key));
	tcw.transferBlockMoves.push_back(createTransferMove(topLeftMiddle,topMiddleBack,key));
	tcw.transferBlockMoves.push_back(createTransferMove(topMiddleBack,topRightMiddle,key));
	tcw.transferBlockMoves.push_back(createTransferMove(topRightMiddle,topMiddleFront,key));
	tcw.transferBlockMoves.push_back(createTransferMove(topMiddleFront,topLeftMiddle,key));
	transferFunctions.push_back(tcw);
	// topCounterClockwise
	TransferFunction tcc;
	tcc.transferFaceMoves.push_back(topCounterClockwise);
	key = BasicBlock::getRKey(0,3,0);
	tcc.transferBlockMoves.push_back(createTransferMove(topLeftFront,topRightFront,key));
	tcc.transferBlockMoves.push_back(createTransferMove(topRightFront,topRightBack,key));
	tcc.transferBlockMoves.push_back(createTransferMove(topRightBack,topLeftBack,key));
	tcc.transferBlockMoves.push_back(createTransferMove(topLeftBack,topLeftFront,key));
	tcc.transferBlockMoves.push_back(createTransferMove(topLeftMiddle,topMiddleFront,key));
	tcc.transferBlockMoves.push_back(createTransferMove(topMiddleFront,topRightMiddle,key));
	tcc.transferBlockMoves.push_back(createTransferMove(topRightMiddle,topMiddleBack,key));
	tcc.transferBlockMoves.push_back(createTransferMove(topMiddleBack,topLeftMiddle,key));
	transferFunctions.push_back(tcc);

	//bottomClockwise
	TransferFunction bcc;
	bcc.transferFaceMoves.push_back(bottomClockwise);
	key = BasicBlock::getRKey(0,3,0);
	bcc.transferBlockMoves.push_back(createTransferMove(bottomLeftFront,bottomRightFront,key));
	bcc.transferBlockMoves.push_back(createTransferMove(bottomRightFront,bottomRightBack,key));
	bcc.transferBlockMoves.push_back(createTransferMove(bottomRightBack,bottomLeftBack,key));
	bcc.transferBlockMoves.push_back(createTransferMove(bottomLeftBack,bottomLeftFront,key));
	bcc.transferBlockMoves.push_back(createTransferMove(bottomLeftMiddle,bottomMiddleFront,key));
	bcc.transferBlockMoves.push_back(createTransferMove(bottomMiddleFront,bottomRightMiddle,key));
	bcc.transferBlockMoves.push_back(createTransferMove(bottomRightMiddle,bottomMiddleBack,key));
	bcc.transferBlockMoves.push_back(createTransferMove(bottomMiddleBack,bottomLeftMiddle,key));
	transferFunctions.push_back(bcc);

	//bottomCounterClockwise
	TransferFunction bcw;
	bcw.transferFaceMoves.push_back(bottomCounterClockwise);
	key = BasicBlock::getRKey(0,1,0);
	bcw.transferBlockMoves.push_back(createTransferMove(bottomLeftFront,bottomLeftBack,key));
	bcw.transferBlockMoves.push_back(createTransferMove(bottomLeftBack,bottomRightBack,key));
	bcw.transferBlockMoves.push_back(createTransferMove(bottomRightBack,bottomRightFront,key));
	bcw.transferBlockMoves.push_back(createTransferMove(bottomRightFront,bottomLeftFront,key));
	bcw.transferBlockMoves.push_back(createTransferMove(bottomLeftMiddle,bottomMiddleBack,key));
	bcw.transferBlockMoves.push_back(createTransferMove(bottomMiddleBack,bottomRightMiddle,key));
	bcw.transferBlockMoves.push_back(createTransferMove(bottomRightMiddle,bottomMiddleFront,key));
	bcw.transferBlockMoves.push_back(createTransferMove(bottomMiddleFront,bottomLeftMiddle,key));
	transferFunctions.push_back(bcw);


	//rightClockwise
	TransferFunction rcw;
	rcw.transferFaceMoves.push_back(rightClockwise);
	key = BasicBlock::getRKey(1,0,0);
	rcw.transferBlockMoves.push_back(createTransferMove(topRightFront,topRightBack,key));
	rcw.transferBlockMoves.push_back(createTransferMove(topRightBack,bottomRightBack,key));
	rcw.transferBlockMoves.push_back(createTransferMove(bottomRightBack,bottomRightFront,key));
	rcw.transferBlockMoves.push_back(createTransferMove(bottomRightFront,topRightFront,key));
	rcw.transferBlockMoves.push_back(createTransferMove(topRightMiddle,middleRightBack,key));
	rcw.transferBlockMoves.push_back(createTransferMove(middleRightBack,bottomRightMiddle,key));
	rcw.transferBlockMoves.push_back(createTransferMove(bottomRightMiddle,middleRightFront,key));
	rcw.transferBlockMoves.push_back(createTransferMove(middleRightFront,topRightMiddle,key));
	transferFunctions.push_back(rcw);

	//rightCounterClockwise
	TransferFunction rcc;
	rcc.transferFaceMoves.push_back(rightCounterClockwise);
	key = BasicBlock::getRKey(3,0,0);
	rcc.transferBlockMoves.push_back(createTransferMove(topRightFront,bottomRightFront,key));
	rcc.transferBlockMoves.push_back(createTransferMove(bottomRightFront,bottomRightBack,key));
	rcc.transferBlockMoves.push_back(createTransferMove(bottomRightBack,topRightBack,key));
	rcc.transferBlockMoves.push_back(createTransferMove(topRightBack,topRightFront,key));
	rcc.transferBlockMoves.push_back(createTransferMove(topRightMiddle,middleRightFront,key));
	rcc.transferBlockMoves.push_back(createTransferMove(middleRightFront,bottomRightMiddle,key));
	rcc.transferBlockMoves.push_back(createTransferMove(bottomRightMiddle,middleRightBack,key));
	rcc.transferBlockMoves.push_back(createTransferMove(middleRightBack,topRightMiddle,key));
	transferFunctions.push_back(rcc);

	//leftClockwise
	TransferFunction lcw;
	lcw.transferFaceMoves.push_back(leftClockwise);
	key = BasicBlock::getRKey(3,0,0);
	lcw.transferBlockMoves.push_back(createTransferMove(topLeftFront,bottomLeftFront,key));
	lcw.transferBlockMoves.push_back(createTransferMove(bottomLeftFront,bottomLeftBack,key));
	lcw.transferBlockMoves.push_back(createTransferMove(bottomLeftBack,topLeftBack,key));
	lcw.transferBlockMoves.push_back(createTransferMove(topLeftBack,topLeftFront,key));
	lcw.transferBlockMoves.push_back(createTransferMove(topLeftMiddle,middleLeftFront,key));
	lcw.transferBlockMoves.push_back(createTransferMove(middleLeftFront,bottomLeftMiddle,key));
	lcw.transferBlockMoves.push_back(createTransferMove(bottomLeftMiddle,middleLeftBack,key));
	lcw.transferBlockMoves.push_back(createTransferMove(middleLeftBack,topLeftMiddle,key));
	transferFunctions.push_back(lcw);

	//leftCounterClockwise
	TransferFunction lcc;
	lcc.transferFaceMoves.push_back(leftCounterClockwise);
	key = BasicBlock::getRKey(1,0,0);
	lcc.transferBlockMoves.push_back(createTransferMove(topLeftFront,topLeftBack,key));
	lcc.transferBlockMoves.push_back(createTransferMove(topLeftBack,bottomLeftBack,key));
	lcc.transferBlockMoves.push_back(createTransferMove(bottomLeftBack,bottomLeftFront,key));
	lcc.transferBlockMoves.push_back(createTransferMove(bottomLeftFront,topLeftFront,key));
	lcc.transferBlockMoves.push_back(createTransferMove(topLeftMiddle,middleLeftBack,key));
	lcc.transferBlockMoves.push_back(createTransferMove(middleLeftBack,bottomLeftMiddle,key));
	lcc.transferBlockMoves.push_back(createTransferMove(bottomLeftMiddle,middleLeftFront,key));
	lcc.transferBlockMoves.push_back(createTransferMove(middleLeftFront,topLeftMiddle,key));
	transferFunctions.push_back(lcc);

	//frontClockwise
	TransferFunction fcw;
	fcw.transferFaceMoves.push_back(frontClockwise);
	key = BasicBlock::getRKey(0,0,1);
	fcw.transferBlockMoves.push_back(createTransferMove(topLeftFront,topRightFront,key));
	fcw.transferBlockMoves.push_back(createTransferMove(topRightFront,bottomRightFront,key));
	fcw.transferBlockMoves.push_back(createTransferMove(bottomRightFront,bottomLeftFront,key));
	fcw.transferBlockMoves.push_back(createTransferMove(bottomLeftFront,topLeftFront,key));
	fcw.transferBlockMoves.push_back(createTransferMove(topMiddleFront,middleRightFront,key));
	fcw.transferBlockMoves.push_back(createTransferMove(middleRightFront,bottomMiddleFront,key));
	fcw.transferBlockMoves.push_back(createTransferMove(bottomMiddleFront,middleLeftFront,key));
	fcw.transferBlockMoves.push_back(createTransferMove(middleLeftFront,topMiddleFront,key));
	transferFunctions.push_back(fcw);

	//frontCounterClockwise
	TransferFunction fcc;
	fcc.transferFaceMoves.push_back(frontCounterClockwise);
	key = BasicBlock::getRKey(0,0,3);
	fcc.transferBlockMoves.push_back(createTransferMove(topLeftFront,bottomLeftFront,key));
	fcc.transferBlockMoves.push_back(createTransferMove(bottomLeftFront,bottomRightFront,key));
	fcc.transferBlockMoves.push_back(createTransferMove(bottomRightFront,topRightFront,key));
	fcc.transferBlockMoves.push_back(createTransferMove(topRightFront,topLeftFront,key));
	fcc.transferBlockMoves.push_back(createTransferMove(topMiddleFront,middleLeftFront,key));
	fcc.transferBlockMoves.push_back(createTransferMove(middleLeftFront,bottomMiddleFront,key));
	fcc.transferBlockMoves.push_back(createTransferMove(bottomMiddleFront,middleRightFront,key));
	fcc.transferBlockMoves.push_back(createTransferMove(middleRightFront,topMiddleFront,key));
	transferFunctions.push_back(fcc);

	//backClockwise
	TransferFunction bacw;
	bacw.transferFaceMoves.push_back(backClockwise);
	key = BasicBlock::getRKey(0,0,3);
	bacw.transferBlockMoves.push_back(createTransferMove(topLeftBack,bottomLeftBack,key));
	bacw.transferBlockMoves.push_back(createTransferMove(bottomLeftBack,bottomRightBack,key));
	bacw.transferBlockMoves.push_back(createTransferMove(bottomRightBack,topRightBack,key));
	bacw.transferBlockMoves.push_back(createTransferMove(topRightBack,topLeftBack,key));
	bacw.transferBlockMoves.push_back(createTransferMove(topMiddleBack,middleLeftBack,key));
	bacw.transferBlockMoves.push_back(createTransferMove(middleLeftBack,bottomMiddleBack,key));
	bacw.transferBlockMoves.push_back(createTransferMove(bottomMiddleBack,middleRightBack,key));
	bacw.transferBlockMoves.push_back(createTransferMove(middleRightBack,topMiddleBack,key));
	transferFunctions.push_back(bacw);

	//backCounterClockwise
	TransferFunction bacc;
	bacc.transferFaceMoves.push_back(backCounterClockwise);
	key = BasicBlock::getRKey(0,0,1);
	bacc.transferBlockMoves.push_back(createTransferMove(topLeftBack,topRightBack,key));
	bacc.transferBlockMoves.push_back(createTransferMove(topRightBack,bottomRightBack,key));
	bacc.transferBlockMoves.push_back(createTransferMove(bottomRightBack,bottomLeftBack,key));
	bacc.transferBlockMoves.push_back(createTransferMove(bottomLeftBack,topLeftBack,key));
	bacc.transferBlockMoves.push_back(createTransferMove(topMiddleBack,middleRightBack,key));
	bacc.transferBlockMoves.push_back(createTransferMove(middleRightBack,bottomMiddleBack,key));
	bacc.transferBlockMoves.push_back(createTransferMove(bottomMiddleBack,middleLeftBack,key));
	bacc.transferBlockMoves.push_back(createTransferMove(middleLeftBack,topMiddleBack,key));
	transferFunctions.push_back(bacc);

}


TransferMoves Cube::createTransferMove(byte oldPosition, byte newPosition, rByte rotate)
{
	TransferMoves newMove;
	newMove.oldPosition = oldPosition;
	newMove.newPosition = newPosition;
	newMove.rotate = rotate;
	return newMove;
}

// Executes the specified move/transfer function on this cube.
void Cube::move(int ti)
{
	Cube oldCube(this); // load a copy of this cube into old cube
	for (size_t i=0;i<transferFunctions[ti].transferBlockMoves.size();++i)
	{
		// get the old block
		int oldBlock = transferFunctions[ti].transferBlockMoves[i].oldPosition;
		int newBlock = transferFunctions[ti].transferBlockMoves[i].newPosition;
		blocks2[newBlock] = oldCube.blocks2[oldBlock];
		BasicBlock::rotate(blocks2[newBlock],transferFunctions[ti].transferBlockMoves[i].rotate);
	}
}


// Adds the specified block to the cube in the specified position
void Cube::addBlock(Positions position, Faces f)
{
	orientVector orientation;
	int blockNum;
	// attempt to create and match the block to a correct orientation.
	for (blockNum = 0; blockNum<20; blockNum++)
	{
		orientation = homeBlocks[blockNum].checkBlock(f);
		// if the returned orientation is invalid, then the homeBlock doesn't match
		if (orientation.isValid())
		{
			// check if the block is already loaded
			for (int i=0;i<20;++i)
			{
				// we know the home block number is "blockNum" so
				// check if any other blocks are claiming this homeBlock
				// ignore the block at the current position.
				if ((i != position) && (blocks2[i].homeBlock == blockNum))
					throw std::runtime_error("The block is already loaded");
			}
			// we found the valid homeBlock
			blocks2[position] = homeBlocks[blockNum].loadBlock(blockNum,position,orientation);

			return;  // we don't need to check the rest of them so just return
		}
	}
	// If we got here, the block wasn't found.
	throw std::runtime_error("Unable to add block.  No orientation matched that color combination");
}

// copys the state of the referenced cube into this one.
void Cube::clone(Cube* c)
{
	for (int i=0;i<20;++i)
	{
		blocks2[i] = c->blocks2[i];
	}
}

// returns true if this Cube and the refrenced one are the same.
bool Cube::equals(Cube* c)
{
	for (int i=0;i<20;++i)
	{
		// check if it is the same block
		if (blocks2[i].homeBlock != c->blocks2[i].homeBlock) 
			return false;

		// check if the block is oriented the same way
		if (!BasicBlock::equalOrientation(blocks2[i],c->blocks2[i])) 
			return false;
	}
	// if we got here then all of the blocks are the same.
	return true;
}

// returns the number of blocks that are in the correct position and orientation
int Cube::solved()
{
	int solvedBlocks=0;
	for (int i=0;i<20;++i)
	{
		if (homeBlocks[blocks2[i].homeBlock].isHome(blocks2[i]))  // needs work
			solvedBlocks++;
	}
	return solvedBlocks;
}


// Checks to make sure this cube has all of the blocks filled in and ready.  If there are any
// blocks missing, it returns an exception that lists them.  addBlock checks for duplicates.
bool Cube::checkCube()
{
	int loadedCubes=0;

	for (int i=0;i<20;++i)
		if (blocks2[i].homeBlock!=0xff)
			loadedCubes++;
	if (loadedCubes!=20)
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

 
int Cube::doOptimumTransferFunction()
{
	return 0;
}

// used by toString to calculate each block color
std::string Cube::getBlockFaceStr(Positions p,Orientation o,bool withBlockNum)
{
	std::stringstream output(" ",std::ios_base::app | std::ios_base::out);
	byte homeBlock = blocks2[p].homeBlock;
	orientVector ov = blocks2[p].orientationVector;
	Faces f = homeBlocks[homeBlock].remapFaces(ov);
	// convert int to string
	if (withBlockNum)
	{
		if (blocks2[p].homeBlock<10)
			output << " ";
		output << (int)blocks2[p].homeBlock;
		output << ",";
	}
	if (o==top)
		output << convertColorToLetter(f.topColor);
	else if (o==bottom)
		output << convertColorToLetter(f.bottomColor);
	else if (o==left)
		output << convertColorToLetter(f.leftColor);
	else if (o==right)
		output << convertColorToLetter(f.rightColor);
	else if (o==front)
		output << convertColorToLetter(f.frontColor);
	else  //o==back
		output << convertColorToLetter(f.backColor);
	output << " ";
	return output.str();
}

// used by toString to translate the "Color" enumeration into a string
std::string Cube::convertColorToLetter(Color c)
{
	if (c==blue) return "B";
	if (c==green) return "G";
	if (c==orange) return "O";
	if (c==red) return "R";
	if (c==white) return "W";
	if (c==yellow) return "Y";
	return "N";
}

// returns the current state of the cube as a string
std::string Cube::toString(bool withBlockNums)
{
	std::string output = "";

	std::string leftOffset;
	if (withBlockNums)
		leftOffset = "                ";
	else
		leftOffset = "       ";

	std::string extraSpace;
	if (withBlockNums)
		extraSpace="   ";
	else
		extraSpace="";
	// allow 4 spaces for each block color.  Two for the block number, and one for the color
	// i'm including the block number at first so that I can make check that it works
	// it should look like 
/*
   yyy
   yyy
   yyy
ooobbbrrr
ooobbbrrr
ooobbbrrr
   www
   www
   www
   ggg
   ggg
   ggg
*/
	// without the block numbers

	// print beginning spaces
	output += leftOffset;
	// output block number and color for yellow face
	output += getBlockFaceStr(bottomLeftBack,back,withBlockNums);
	output += getBlockFaceStr(bottomMiddleBack,back,withBlockNums);
	output += getBlockFaceStr(bottomRightBack,back,withBlockNums);

	output += "\n" + leftOffset;
	output += getBlockFaceStr(middleLeftBack,back,withBlockNums);
	output += extraSpace + "Y ";
	output += getBlockFaceStr(middleRightBack,back,withBlockNums);

	output += "\n" + leftOffset;
	output += getBlockFaceStr(topLeftBack,back,withBlockNums);
	output += getBlockFaceStr(topMiddleBack,back,withBlockNums);
	output += getBlockFaceStr(topRightBack,back,withBlockNums);


	// output orange back row
	output += "\n\n";
	output += getBlockFaceStr(bottomLeftBack,left,withBlockNums);
	output += getBlockFaceStr(middleLeftBack,left,withBlockNums);
	output += getBlockFaceStr(topLeftBack,left,withBlockNums);

	output += " "; 
	// output blue back row
	output += getBlockFaceStr(topLeftBack,top,withBlockNums);
	output += getBlockFaceStr(topMiddleBack,top,withBlockNums);
	output += getBlockFaceStr(topRightBack,top,withBlockNums);

	output += " "; 
	// output red back row
	output += getBlockFaceStr(topRightBack,right,withBlockNums);
	output += getBlockFaceStr(middleRightBack,right,withBlockNums);
	output += getBlockFaceStr(bottomRightBack,right,withBlockNums);


	// output orange middle row
	output += "\n";
	output += getBlockFaceStr(bottomLeftMiddle,left,withBlockNums);
	output += extraSpace + "O ";
	output += getBlockFaceStr(topLeftMiddle,left,withBlockNums);

	output += " "; 
	// output blue middle row
	output += getBlockFaceStr(topLeftMiddle,top,withBlockNums);
	output += extraSpace + "B ";
	output += getBlockFaceStr(topRightMiddle,top,withBlockNums);

	output += " "; 
	// output red middle row
	output += getBlockFaceStr(topRightMiddle,right,withBlockNums);
	output += extraSpace + "R ";
	output += getBlockFaceStr(bottomRightMiddle,right,withBlockNums);
	

	// output orange front row
	output += "\n";
	output += getBlockFaceStr(bottomLeftFront,left,withBlockNums);
	output += getBlockFaceStr(middleLeftFront,left,withBlockNums);
	output += getBlockFaceStr(topLeftFront,left,withBlockNums);

	output += " "; 
	// output blue front row
	output += getBlockFaceStr(topLeftFront,top,withBlockNums);
	output += getBlockFaceStr(topMiddleFront,top,withBlockNums);
	output += getBlockFaceStr(topRightFront,top,withBlockNums);

	output += " "; 
	// output red front row
	output += getBlockFaceStr(topRightFront,right,withBlockNums);
	output += getBlockFaceStr(middleRightFront,right,withBlockNums);
	output += getBlockFaceStr(bottomRightFront,right,withBlockNums);


	// output block number and color for white face
	output += "\n\n" + leftOffset;
	output += getBlockFaceStr(topLeftFront,front,withBlockNums);
	output += getBlockFaceStr(topMiddleFront,front,withBlockNums);
	output += getBlockFaceStr(topRightFront,front,withBlockNums);

	output += "\n" + leftOffset;
	output += getBlockFaceStr(middleLeftFront,front,withBlockNums);
	output += extraSpace + "W ";
	output += getBlockFaceStr(middleRightFront,front,withBlockNums);

	output += "\n" + leftOffset;
	output += getBlockFaceStr(bottomLeftFront,front,withBlockNums);
	output += getBlockFaceStr(bottomMiddleFront,front,withBlockNums);
	output += getBlockFaceStr(bottomRightFront,front,withBlockNums);


	// output block number and color for green face
	output += "\n\n" + leftOffset;
	output += getBlockFaceStr(bottomLeftFront,bottom,withBlockNums);
	output += getBlockFaceStr(bottomMiddleFront,bottom,withBlockNums);
	output += getBlockFaceStr(bottomRightFront,bottom,withBlockNums);

	output += "\n" + leftOffset;
	output += getBlockFaceStr(bottomLeftMiddle,bottom,withBlockNums);
	output += extraSpace + "G ";
	output += getBlockFaceStr(bottomRightMiddle,bottom,withBlockNums);

	output += "\n" + leftOffset;
	output += getBlockFaceStr(bottomLeftBack,bottom,withBlockNums);
	output += getBlockFaceStr(bottomMiddleBack,bottom,withBlockNums);
	output += getBlockFaceStr(bottomRightBack,bottom,withBlockNums);
	output += "\n\n";

	return output;
}





