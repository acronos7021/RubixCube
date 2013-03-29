#include <sstream>
#include <vector>
#include <time.h>
#include "Cube.h"

// makes the static variable "initalized" be set to false
// the first time Cube is instantiated.
bool Cube::initialized = false;
BasicBlock Cube::homeBlocks[20];
std::vector<TransferFunction> Cube::transferFunctions;
std::map<std::vector<TransferMoves>,int> Cube::reverseTransferFunctionMap;

Cube::Cube(void)
{
	if (!initialized)
	{
		initHomeBlocks();
		initBasicTransferFunctions();
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
		blocks2[i].position = Positions::noPositionMatches;
		blocks2[i].orientationVector = orientVector(Rotator(Orientation::top),Rotator(Orientation::top));
	}
}

// the possible positions of the blocks.  Organized as [height][width][depth]
void Cube::initHomeBlocks()
{
	// there are 20 blocks.
	//										the orientations are set to home by default
									//		 	     top,bottom,left,right,front,back
									//			     blue,green,orange,red,white,yellow
	homeBlocks[0].initBlock(Positions::topLeftFront,		Faces(Color::blue,Color::none,Color::orange,Color::none,Color::white,Color::none));
	homeBlocks[1].initBlock(Positions::topLeftMiddle,		Faces(Color::blue,Color::none,Color::orange,Color::none,Color::none, Color::none));
	homeBlocks[2].initBlock(Positions::topLeftBack,			Faces(Color::blue,Color::none,Color::orange,Color::none,Color::none, Color::yellow));
	homeBlocks[3].initBlock(Positions::topMiddleFront,		Faces(Color::blue,Color::none,Color::none,  Color::none,Color::white,Color::none));
	homeBlocks[4].initBlock(Positions::topMiddleBack,		Faces(Color::blue,Color::none,Color::none,Color::none,Color::none,Color::yellow));
	homeBlocks[5].initBlock(Positions::topRightFront,		Faces(Color::blue,Color::none,Color::none,Color::red,Color::white,Color::none));
	homeBlocks[6].initBlock(Positions::topRightMiddle,		Faces(Color::blue,Color::none,Color::none,Color::red,Color::none,Color::none));
	homeBlocks[7].initBlock(Positions::topRightBack,		Faces(Color::blue,Color::none,Color::none,Color::red,Color::none,Color::yellow));
	homeBlocks[8].initBlock(Positions::middleLeftFront,		Faces(Color::none,Color::none,Color::orange,Color::none,Color::white,Color::none));
	homeBlocks[9].initBlock(Positions::middleLeftBack,		Faces(Color::none,Color::none,Color::orange,Color::none,Color::none,Color::yellow));
	homeBlocks[10].initBlock(Positions::middleRightFront,	Faces(Color::none,Color::none,Color::none,Color::red,Color::white,Color::none));
	homeBlocks[11].initBlock(Positions::middleRightBack,	Faces(Color::none,Color::none,Color::none,Color::red,Color::none,Color::yellow));
	homeBlocks[12].initBlock(Positions::bottomLeftFront,	Faces(Color::none,Color::green,Color::orange,Color::none,Color::white,Color::none));
	homeBlocks[13].initBlock(Positions::bottomLeftMiddle,	Faces(Color::none,Color::green,Color::orange,Color::none,Color::none,Color::none));
	homeBlocks[14].initBlock(Positions::bottomLeftBack,		Faces(Color::none,Color::green,Color::orange,Color::none,Color::none,Color::yellow));
	homeBlocks[15].initBlock(Positions::bottomMiddleFront,	Faces(Color::none,Color::green,Color::none,Color::none,Color::white,Color::none));
	homeBlocks[16].initBlock(Positions::bottomMiddleBack,	Faces(Color::none,Color::green,Color::none,Color::none,Color::none,Color::yellow));
	homeBlocks[17].initBlock(Positions::bottomRightMiddle,	Faces(Color::none,Color::green,Color::none,Color::red,Color::white,Color::none));
	homeBlocks[18].initBlock(Positions::bottomRightFront,	Faces(Color::none,Color::green,Color::none,Color::red,Color::none,Color::none));
	homeBlocks[19].initBlock(Positions::bottomRightBack,	Faces(Color::none,Color::green,Color::none,Color::red,Color::none,Color::yellow));
}
	
void Cube::initBasicTransferFunctions()
{
	// the first 12 transfer functions are the basic moves on each face
	// each block has to lead to the next without deleting the previous
	// for now we will use a vector because its easier.  Optomize later
	
	// topClockwisetop
	TransferFunction tcw;
	tcw.transferFaceMoves.push_back(topClockwise);
	rByte key = BasicBlock::getRKey(0,1,0);
	tcw.transferBlockMoves.push_back(createTransferMove(Positions::topLeftFront,Positions::topLeftBack,key));
	tcw.transferBlockMoves.push_back(createTransferMove(Positions::topLeftBack,Positions::topRightBack,key));
	tcw.transferBlockMoves.push_back(createTransferMove(Positions::topRightBack,Positions::topRightFront,key));
	tcw.transferBlockMoves.push_back(createTransferMove(Positions::topRightFront,Positions::topLeftFront,key));
	tcw.transferBlockMoves.push_back(createTransferMove(Positions::topLeftMiddle,Positions::topMiddleBack,key));
	tcw.transferBlockMoves.push_back(createTransferMove(Positions::topMiddleBack,Positions::topRightMiddle,key));
	tcw.transferBlockMoves.push_back(createTransferMove(Positions::topRightMiddle,Positions::topMiddleFront,key));
	tcw.transferBlockMoves.push_back(createTransferMove(Positions::topMiddleFront,Positions::topLeftMiddle,key));
	transferFunctions.push_back(tcw);
	reverseTransferFunctionMap.insert(reverseTransferFunctionPair(tcw.transferBlockMoves,0));
	
	// topCounterClockwise
	TransferFunction tcc;
	tcc.transferFaceMoves.push_back(topCounterClockwise);
	key = BasicBlock::getRKey(0,3,0);
	tcc.transferBlockMoves.push_back(createTransferMove(Positions::topLeftFront,Positions::topRightFront,key));
	tcc.transferBlockMoves.push_back(createTransferMove(Positions::topRightFront,Positions::topRightBack,key));
	tcc.transferBlockMoves.push_back(createTransferMove(Positions::topRightBack,Positions::topLeftBack,key));
	tcc.transferBlockMoves.push_back(createTransferMove(Positions::topLeftBack,Positions::topLeftFront,key));
	tcc.transferBlockMoves.push_back(createTransferMove(Positions::topLeftMiddle,Positions::topMiddleFront,key));
	tcc.transferBlockMoves.push_back(createTransferMove(Positions::topMiddleFront,Positions::topRightMiddle,key));
	tcc.transferBlockMoves.push_back(createTransferMove(Positions::topRightMiddle,Positions::topMiddleBack,key));
	tcc.transferBlockMoves.push_back(createTransferMove(Positions::topMiddleBack,Positions::topLeftMiddle,key));
	transferFunctions.push_back(tcc);
	reverseTransferFunctionMap.insert(reverseTransferFunctionPair(tcc.transferBlockMoves,1));

	//bottomClockwise
	TransferFunction bcc;
	bcc.transferFaceMoves.push_back(bottomClockwise);
	key = BasicBlock::getRKey(0,3,0);
	bcc.transferBlockMoves.push_back(createTransferMove(Positions::bottomLeftFront,Positions::bottomRightFront,key));
	bcc.transferBlockMoves.push_back(createTransferMove(Positions::bottomRightFront,Positions::bottomRightBack,key));
	bcc.transferBlockMoves.push_back(createTransferMove(Positions::bottomRightBack,Positions::bottomLeftBack,key));
	bcc.transferBlockMoves.push_back(createTransferMove(Positions::bottomLeftBack,Positions::bottomLeftFront,key));
	bcc.transferBlockMoves.push_back(createTransferMove(Positions::bottomLeftMiddle,Positions::bottomMiddleFront,key));
	bcc.transferBlockMoves.push_back(createTransferMove(Positions::bottomMiddleFront,Positions::bottomRightMiddle,key));
	bcc.transferBlockMoves.push_back(createTransferMove(Positions::bottomRightMiddle,Positions::bottomMiddleBack,key));
	bcc.transferBlockMoves.push_back(createTransferMove(Positions::bottomMiddleBack,Positions::bottomLeftMiddle,key));
	transferFunctions.push_back(bcc);
	reverseTransferFunctionMap.insert(reverseTransferFunctionPair(bcc.transferBlockMoves,2));

	//bottomCounterClockwise
	TransferFunction bcw;
	bcw.transferFaceMoves.push_back(bottomCounterClockwise);
	key = BasicBlock::getRKey(0,1,0);
	bcw.transferBlockMoves.push_back(createTransferMove(Positions::bottomLeftFront,Positions::bottomLeftBack,key));
	bcw.transferBlockMoves.push_back(createTransferMove(Positions::bottomLeftBack,Positions::bottomRightBack,key));
	bcw.transferBlockMoves.push_back(createTransferMove(Positions::bottomRightBack,Positions::bottomRightFront,key));
	bcw.transferBlockMoves.push_back(createTransferMove(Positions::bottomRightFront,Positions::bottomLeftFront,key));
	bcw.transferBlockMoves.push_back(createTransferMove(Positions::bottomLeftMiddle,Positions::bottomMiddleBack,key));
	bcw.transferBlockMoves.push_back(createTransferMove(Positions::bottomMiddleBack,Positions::bottomRightMiddle,key));
	bcw.transferBlockMoves.push_back(createTransferMove(Positions::bottomRightMiddle,Positions::bottomMiddleFront,key));
	bcw.transferBlockMoves.push_back(createTransferMove(Positions::bottomMiddleFront,Positions::bottomLeftMiddle,key));
	transferFunctions.push_back(bcw);
	reverseTransferFunctionMap.insert(reverseTransferFunctionPair(bcw.transferBlockMoves,3));


	//rightClockwise
	TransferFunction rcw;
	rcw.transferFaceMoves.push_back(rightClockwise);
	key = BasicBlock::getRKey(1,0,0);
	rcw.transferBlockMoves.push_back(createTransferMove(Positions::topRightFront,Positions::topRightBack,key));
	rcw.transferBlockMoves.push_back(createTransferMove(Positions::topRightBack,Positions::bottomRightBack,key));
	rcw.transferBlockMoves.push_back(createTransferMove(Positions::bottomRightBack,Positions::bottomRightFront,key));
	rcw.transferBlockMoves.push_back(createTransferMove(Positions::bottomRightFront,Positions::topRightFront,key));
	rcw.transferBlockMoves.push_back(createTransferMove(Positions::topRightMiddle,Positions::middleRightBack,key));
	rcw.transferBlockMoves.push_back(createTransferMove(Positions::middleRightBack,Positions::bottomRightMiddle,key));
	rcw.transferBlockMoves.push_back(createTransferMove(Positions::bottomRightMiddle,Positions::middleRightFront,key));
	rcw.transferBlockMoves.push_back(createTransferMove(Positions::middleRightFront,Positions::topRightMiddle,key));
	transferFunctions.push_back(rcw);
	reverseTransferFunctionMap.insert(reverseTransferFunctionPair(rcw.transferBlockMoves,4));

	//rightCounterClockwise
	TransferFunction rcc;
	rcc.transferFaceMoves.push_back(rightCounterClockwise);
	key = BasicBlock::getRKey(3,0,0);
	rcc.transferBlockMoves.push_back(createTransferMove(Positions::topRightFront,Positions::bottomRightFront,key));
	rcc.transferBlockMoves.push_back(createTransferMove(Positions::bottomRightFront,Positions::bottomRightBack,key));
	rcc.transferBlockMoves.push_back(createTransferMove(Positions::bottomRightBack,Positions::topRightBack,key));
	rcc.transferBlockMoves.push_back(createTransferMove(Positions::topRightBack,Positions::topRightFront,key));
	rcc.transferBlockMoves.push_back(createTransferMove(Positions::topRightMiddle,Positions::middleRightFront,key));
	rcc.transferBlockMoves.push_back(createTransferMove(Positions::middleRightFront,Positions::bottomRightMiddle,key));
	rcc.transferBlockMoves.push_back(createTransferMove(Positions::bottomRightMiddle,Positions::middleRightBack,key));
	rcc.transferBlockMoves.push_back(createTransferMove(Positions::middleRightBack,Positions::topRightMiddle,key));
	transferFunctions.push_back(rcc);
	reverseTransferFunctionMap.insert(reverseTransferFunctionPair(rcc.transferBlockMoves,5));

	//leftClockwise
	TransferFunction lcw;
	lcw.transferFaceMoves.push_back(leftClockwise);
	key = BasicBlock::getRKey(3,0,0);
	lcw.transferBlockMoves.push_back(createTransferMove(Positions::topLeftFront,Positions::bottomLeftFront,key));
	lcw.transferBlockMoves.push_back(createTransferMove(Positions::bottomLeftFront,Positions::bottomLeftBack,key));
	lcw.transferBlockMoves.push_back(createTransferMove(Positions::bottomLeftBack,Positions::topLeftBack,key));
	lcw.transferBlockMoves.push_back(createTransferMove(Positions::topLeftBack,Positions::topLeftFront,key));
	lcw.transferBlockMoves.push_back(createTransferMove(Positions::topLeftMiddle,Positions::middleLeftFront,key));
	lcw.transferBlockMoves.push_back(createTransferMove(Positions::middleLeftFront,Positions::bottomLeftMiddle,key));
	lcw.transferBlockMoves.push_back(createTransferMove(Positions::bottomLeftMiddle,Positions::middleLeftBack,key));
	lcw.transferBlockMoves.push_back(createTransferMove(Positions::middleLeftBack,Positions::topLeftMiddle,key));
	transferFunctions.push_back(lcw);
	reverseTransferFunctionMap.insert(reverseTransferFunctionPair(lcw.transferBlockMoves,6));

	//leftCounterClockwise
	TransferFunction lcc;
	lcc.transferFaceMoves.push_back(leftCounterClockwise);
	key = BasicBlock::getRKey(1,0,0);
	lcc.transferBlockMoves.push_back(createTransferMove(Positions::topLeftFront,Positions::topLeftBack,key));
	lcc.transferBlockMoves.push_back(createTransferMove(Positions::topLeftBack,Positions::bottomLeftBack,key));
	lcc.transferBlockMoves.push_back(createTransferMove(Positions::bottomLeftBack,Positions::bottomLeftFront,key));
	lcc.transferBlockMoves.push_back(createTransferMove(Positions::bottomLeftFront,Positions::topLeftFront,key));
	lcc.transferBlockMoves.push_back(createTransferMove(Positions::topLeftMiddle,Positions::middleLeftBack,key));
	lcc.transferBlockMoves.push_back(createTransferMove(Positions::middleLeftBack,Positions::bottomLeftMiddle,key));
	lcc.transferBlockMoves.push_back(createTransferMove(Positions::bottomLeftMiddle,Positions::middleLeftFront,key));
	lcc.transferBlockMoves.push_back(createTransferMove(Positions::middleLeftFront,Positions::topLeftMiddle,key));
	transferFunctions.push_back(lcc);
	reverseTransferFunctionMap.insert(reverseTransferFunctionPair(lcc.transferBlockMoves,7));

	//frontClockwise
	TransferFunction fcw;
	fcw.transferFaceMoves.push_back(frontClockwise);
	key = BasicBlock::getRKey(0,0,1);
	fcw.transferBlockMoves.push_back(createTransferMove(Positions::topLeftFront,Positions::topRightFront,key));
	fcw.transferBlockMoves.push_back(createTransferMove(Positions::topRightFront,Positions::bottomRightFront,key));
	fcw.transferBlockMoves.push_back(createTransferMove(Positions::bottomRightFront,Positions::bottomLeftFront,key));
	fcw.transferBlockMoves.push_back(createTransferMove(Positions::bottomLeftFront,Positions::topLeftFront,key));
	fcw.transferBlockMoves.push_back(createTransferMove(Positions::topMiddleFront,Positions::middleRightFront,key));
	fcw.transferBlockMoves.push_back(createTransferMove(Positions::middleRightFront,Positions::bottomMiddleFront,key));
	fcw.transferBlockMoves.push_back(createTransferMove(Positions::bottomMiddleFront,Positions::middleLeftFront,key));
	fcw.transferBlockMoves.push_back(createTransferMove(Positions::middleLeftFront,Positions::topMiddleFront,key));
	transferFunctions.push_back(fcw);
	reverseTransferFunctionMap.insert(reverseTransferFunctionPair(fcw.transferBlockMoves,8));

	//frontCounterClockwise
	TransferFunction fcc;
	fcc.transferFaceMoves.push_back(frontCounterClockwise);
	key = BasicBlock::getRKey(0,0,3);
	fcc.transferBlockMoves.push_back(createTransferMove(Positions::topLeftFront,Positions::bottomLeftFront,key));
	fcc.transferBlockMoves.push_back(createTransferMove(Positions::bottomLeftFront,Positions::bottomRightFront,key));
	fcc.transferBlockMoves.push_back(createTransferMove(Positions::bottomRightFront,Positions::topRightFront,key));
	fcc.transferBlockMoves.push_back(createTransferMove(Positions::topRightFront,Positions::topLeftFront,key));
	fcc.transferBlockMoves.push_back(createTransferMove(Positions::topMiddleFront,Positions::middleLeftFront,key));
	fcc.transferBlockMoves.push_back(createTransferMove(Positions::middleLeftFront,Positions::bottomMiddleFront,key));
	fcc.transferBlockMoves.push_back(createTransferMove(Positions::bottomMiddleFront,Positions::middleRightFront,key));
	fcc.transferBlockMoves.push_back(createTransferMove(Positions::middleRightFront,Positions::topMiddleFront,key));
	transferFunctions.push_back(fcc);
	reverseTransferFunctionMap.insert(reverseTransferFunctionPair(fcc.transferBlockMoves,9));

	//backClockwise
	TransferFunction bacw;
	bacw.transferFaceMoves.push_back(backClockwise);
	key = BasicBlock::getRKey(0,0,3);
	bacw.transferBlockMoves.push_back(createTransferMove(Positions::topLeftBack,Positions::bottomLeftBack,key));
	bacw.transferBlockMoves.push_back(createTransferMove(Positions::bottomLeftBack,Positions::bottomRightBack,key));
	bacw.transferBlockMoves.push_back(createTransferMove(Positions::bottomRightBack,Positions::topRightBack,key));
	bacw.transferBlockMoves.push_back(createTransferMove(Positions::topRightBack,Positions::topLeftBack,key));
	bacw.transferBlockMoves.push_back(createTransferMove(Positions::topMiddleBack,Positions::middleLeftBack,key));
	bacw.transferBlockMoves.push_back(createTransferMove(Positions::middleLeftBack,Positions::bottomMiddleBack,key));
	bacw.transferBlockMoves.push_back(createTransferMove(Positions::bottomMiddleBack,Positions::middleRightBack,key));
	bacw.transferBlockMoves.push_back(createTransferMove(Positions::middleRightBack,Positions::topMiddleBack,key));
	transferFunctions.push_back(bacw);
	reverseTransferFunctionMap.insert(reverseTransferFunctionPair(bacw.transferBlockMoves,10));

	//backCounterClockwise
	TransferFunction bacc;
	bacc.transferFaceMoves.push_back(backCounterClockwise);
	key = BasicBlock::getRKey(0,0,1);
	bacc.transferBlockMoves.push_back(createTransferMove(Positions::topLeftBack,Positions::topRightBack,key));
	bacc.transferBlockMoves.push_back(createTransferMove(Positions::topRightBack,Positions::bottomRightBack,key));
	bacc.transferBlockMoves.push_back(createTransferMove(Positions::bottomRightBack,Positions::bottomLeftBack,key));
	bacc.transferBlockMoves.push_back(createTransferMove(Positions::bottomLeftBack,Positions::topLeftBack,key));
	bacc.transferBlockMoves.push_back(createTransferMove(Positions::topMiddleBack,Positions::middleRightBack,key));
	bacc.transferBlockMoves.push_back(createTransferMove(Positions::middleRightBack,Positions::bottomMiddleBack,key));
	bacc.transferBlockMoves.push_back(createTransferMove(Positions::bottomMiddleBack,Positions::middleLeftBack,key));
	bacc.transferBlockMoves.push_back(createTransferMove(Positions::middleLeftBack,Positions::topMiddleBack,key));
	transferFunctions.push_back(bacc);
	reverseTransferFunctionMap.insert(reverseTransferFunctionPair(bacc.transferBlockMoves,11));
}

TransferMoves Cube::createTransferMove(Positions oldPosition, Positions newPosition, rByte rotate)
{
	TransferMoves newMove;
	newMove.oldPosition = oldPosition;
	newMove.newPosition = newPosition;
	newMove.rotate = rotate;
	return newMove;
}


void Cube::calculateAdvancedTransferFunctions()
{
	int alreadyExistsCount=0;
	// totalSize of the transferFunctions vector
	int totalSize=(8 * sizeof(TransferMoves) + sizeof(Moves) )*12; // the 12 basic moves each require 3 bytes and move 8 blocks and 1 facemove

	Cube testCube;
	Cube solvedCube;
	Cube workingCube;
	solvedCube.SetCubeToSolved();
	int moves=1; // the first run through simulates the first move
	time_t startTimer;  // used to determine the actual time required to process a move
	time_t endTimer;
	size_t beginGenerationIndex=0;  // used to keep from having to loop over transfer functions that have already been calculated.
	size_t endGenerationIndex;
	while(totalSize < 100000000) 	// keep adding new transfer functions until the totalSize > 100MB
	{
		// apply the basic moves to each transfer function in the last generation.  This should check every possible
		// combination of moves and add one more move to that set.


		startTimer=time(nullptr);
		int lastGenerationAdditions=0;
		int positionsConsideredInLastGeneration=0;
		
		endGenerationIndex = transferFunctions.size();  // I don't want the size changing as the loop progresses
		for (size_t tfIndex=beginGenerationIndex; tfIndex < endGenerationIndex; ++tfIndex)
		{
			// only work on the last generation.  Otherwise, we duplicate all the previous generations again
			if (transferFunctions[tfIndex].transferFaceMoves.size()==moves)
			{
				workingCube.SetCubeToSolved();
				workingCube.move(tfIndex);
				// apply each basic move to the current function to calculate the next move
				for (size_t basicIndex=0; basicIndex < 12; ++basicIndex)
				{
					++positionsConsideredInLastGeneration;
					// apply move to set
					testCube.clone(&workingCube);
					testCube.move(basicIndex);
					// calculate the difference between a solved cube and the testCube
					std::vector<TransferMoves> newBlockMoves = calculateTransferMove(solvedCube,testCube);

					if (newBlockMoves.size()==0)
					{
						// this series of moves doesn't do anything so ignore it
						++alreadyExistsCount;
					}
					else
					{
						// attempt to insert it into the reverseTransferFunctionMap
						// this also checks to see if the function already exists.  If so ret.second is false
						// and we don't need to add it again.
						std::pair<std::map<std::vector<TransferMoves>,int>::iterator,bool> ret = reverseTransferFunctionMap.insert(reverseTransferFunctionPair(newBlockMoves,transferFunctions.size()));

						if (ret.second)
						{
							// the insertion succeeded so the function was not already in the database.
							// add the new transfer function.
							TransferFunction newTF;
							std::vector<Moves> newTFface;
							newTF.transferBlockMoves = newBlockMoves;
							// add the face moves that were already calculated
							for (size_t i=0; i < transferFunctions[tfIndex].transferFaceMoves.size(); ++i)
							{
								newTFface.push_back(transferFunctions[tfIndex].transferFaceMoves[i]);
							}
							// add the new face move
							newTFface.push_back((Moves)basicIndex);
							newTF.transferFaceMoves = newTFface;
							transferFunctions.push_back(newTF);
							totalSize+= sizeof(TransferMoves)  * newTF.transferBlockMoves.size();
							totalSize+= sizeof(Moves) * newTF.transferFaceMoves.size();
							lastGenerationAdditions++;
						}
						else
						{
							++alreadyExistsCount;
						}
					}
				}
			}
		}
		beginGenerationIndex=endGenerationIndex;
		endTimer=time(nullptr);
		double seconds = difftime(startTimer,endTimer);
		moves++; // just so that I know for testing how many moves deep we got
	}
}

// it is important that these are always sorted in the same way so that they can be compared
std::vector<TransferMoves> Cube::calculateTransferMove(Cube oldCube,Cube newCube)
{
	// find each block that is different in the newCube
	std::vector<TransferMoves> theMoves;
	for (int oldBlockIndex=0; oldBlockIndex < 20; ++oldBlockIndex)
	{
		if (!BasicBlock::equalBlocks(oldCube.blocks2[oldBlockIndex], newCube.blocks2[oldBlockIndex]))
		{
			// the two blocks aren't equal, so find the original position of the new block.
			for (int newBlockIndex = 0; newBlockIndex < 20; ++newBlockIndex)
			{
				if (newCube.blocks2[newBlockIndex].homeBlock == oldCube.blocks2[oldBlockIndex].homeBlock)
				{
					// found the position of the block in the oldCube
					TransferMoves thisMove;
					thisMove.oldPosition = (Positions) oldBlockIndex;
					thisMove.newPosition = (Positions) newBlockIndex;
					thisMove.rotate = BasicBlock::deRotate(oldCube.blocks2[oldBlockIndex].orientationVector
														  ,newCube.blocks2[newBlockIndex].orientationVector);
					theMoves.push_back(thisMove);
					break;  // it's found so no need to continue the loop
				}
			}
		}
	}
	return theMoves;
}

bool Cube::isSameTransferMoves(std::vector<TransferMoves> mv1, std::vector<TransferMoves> mv2)
{
	if (mv1.size()!=mv2.size()) return false;
	for (size_t i=0;i<mv1.size();++i)
	{
		if (mv1[i].newPosition != mv2[i].newPosition) return false;
		if (mv1[i].oldPosition != mv2[i].oldPosition) return false;
		if (mv1[i].rotate != mv2[i].rotate) return false;
	}
	// we didn't find any differences so return true
	return true;
}

// Executes the specified move/transfer function on this cube.
void Cube::move(int ti)
{
	Cube oldCube(this); // load a copy of this cube into old cube
	for (size_t i=0;i<transferFunctions[ti].transferBlockMoves.size();++i)
	{
		// get the old block
		int oldBlock = (byte) transferFunctions[ti].transferBlockMoves[i].oldPosition;
		int newBlock = (byte) transferFunctions[ti].transferBlockMoves[i].newPosition;
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
				if ((i != (byte) position) && (blocks2[i].homeBlock == blockNum))
					throw std::runtime_error("The block is already loaded");
			}
			// we found the valid homeBlock
			blocks2[(byte) position] = homeBlocks[blockNum].loadBlock(blockNum,position,orientation);

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
int Cube::unSolved()
{
	int unSolvedBlocks=20;
	for (int i=0;i<20;++i)
		if ((blocks2[i].homeBlock==i)&&(orientVector::isHome(blocks2[i].orientationVector)))
			unSolvedBlocks--;
	return unSolvedBlocks;
}

int Cube::unSolved(Cube c)
{
	int unSolvedBlocks=20;
	for (int i=0;i<20;++i)
		if ((c.blocks2[i].homeBlock==i)&&(orientVector::isHome(c.blocks2[i].orientationVector)))
			unSolvedBlocks--;
	return unSolvedBlocks;
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
	byte homeBlock = blocks2[(byte) p].homeBlock;
	orientVector ov = blocks2[(byte) p].orientationVector;
	Faces f = homeBlocks[homeBlock].remapFaces(ov);
	// convert int to string
	if (withBlockNum)
	{
		if (blocks2[(byte) p].homeBlock<10)
			output << " ";
		output << (int)blocks2[(byte) p].homeBlock;
		output << ",";
	}
	if (o==Orientation::top)
		output << convertColorToLetter(f.topColor);
	else if (o==Orientation::bottom)
		output << convertColorToLetter(f.bottomColor);
	else if (o==Orientation::left)
		output << convertColorToLetter(f.leftColor);
	else if (o==Orientation::right)
		output << convertColorToLetter(f.rightColor);
	else if (o==Orientation::front)
		output << convertColorToLetter(f.frontColor);
	else  //o==back
		output << convertColorToLetter(f.backColor);
	output << " ";
	return output.str();
}

// used by toString to translate the "Color" enumeration into a string
std::string Cube::convertColorToLetter(Color c)
{
	if (c==Color::blue) return "B";
	if (c==Color::green) return "G";
	if (c==Color::orange) return "O";
	if (c==Color::red) return "R";
	if (c==Color::white) return "W";
	if (c==Color::yellow) return "Y";
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
	output += getBlockFaceStr(Positions::bottomLeftBack,Orientation::back,withBlockNums);
	output += getBlockFaceStr(Positions::bottomMiddleBack,Orientation::back,withBlockNums);
	output += getBlockFaceStr(Positions::bottomRightBack,Orientation::back,withBlockNums);

	output += "\n" + leftOffset;
	output += getBlockFaceStr(Positions::middleLeftBack,Orientation::back,withBlockNums);
	output += extraSpace + "Y ";
	output += getBlockFaceStr(Positions::middleRightBack,Orientation::back,withBlockNums);

	output += "\n" + leftOffset;
	output += getBlockFaceStr(Positions::topLeftBack,Orientation::back,withBlockNums);
	output += getBlockFaceStr(Positions::topMiddleBack,Orientation::back,withBlockNums);
	output += getBlockFaceStr(Positions::topRightBack,Orientation::back,withBlockNums);


	// output orange back row
	output += "\n\n";
	output += getBlockFaceStr(Positions::bottomLeftBack,Orientation::left,withBlockNums);
	output += getBlockFaceStr(Positions::middleLeftBack,Orientation::left,withBlockNums);
	output += getBlockFaceStr(Positions::topLeftBack,Orientation::left,withBlockNums);

	output += " "; 
	// output blue back row
	output += getBlockFaceStr(Positions::topLeftBack,Orientation::top,withBlockNums);
	output += getBlockFaceStr(Positions::topMiddleBack,Orientation::top,withBlockNums);
	output += getBlockFaceStr(Positions::topRightBack,Orientation::top,withBlockNums);

	output += " "; 
	// output red back row
	output += getBlockFaceStr(Positions::topRightBack,Orientation::right,withBlockNums);
	output += getBlockFaceStr(Positions::middleRightBack,Orientation::right,withBlockNums);
	output += getBlockFaceStr(Positions::bottomRightBack,Orientation::right,withBlockNums);


	// output orange middle row
	output += "\n";
	output += getBlockFaceStr(Positions::bottomLeftMiddle,Orientation::left,withBlockNums);
	output += extraSpace + "O ";
	output += getBlockFaceStr(Positions::topLeftMiddle,Orientation::left,withBlockNums);

	output += " "; 
	// output blue middle row
	output += getBlockFaceStr(Positions::topLeftMiddle,Orientation::top,withBlockNums);
	output += extraSpace + "B ";
	output += getBlockFaceStr(Positions::topRightMiddle,Orientation::top,withBlockNums);

	output += " "; 
	// output red middle row
	output += getBlockFaceStr(Positions::topRightMiddle,Orientation::right,withBlockNums);
	output += extraSpace + "R ";
	output += getBlockFaceStr(Positions::bottomRightMiddle,Orientation::right,withBlockNums);
	

	// output orange front row
	output += "\n";
	output += getBlockFaceStr(Positions::bottomLeftFront,Orientation::left,withBlockNums);
	output += getBlockFaceStr(Positions::middleLeftFront,Orientation::left,withBlockNums);
	output += getBlockFaceStr(Positions::topLeftFront,Orientation::left,withBlockNums);

	output += " "; 
	// output blue front row
	output += getBlockFaceStr(Positions::topLeftFront,Orientation::top,withBlockNums);
	output += getBlockFaceStr(Positions::topMiddleFront,Orientation::top,withBlockNums);
	output += getBlockFaceStr(Positions::topRightFront,Orientation::top,withBlockNums);

	output += " "; 
	// output red front row
	output += getBlockFaceStr(Positions::topRightFront,Orientation::right,withBlockNums);
	output += getBlockFaceStr(Positions::middleRightFront,Orientation::right,withBlockNums);
	output += getBlockFaceStr(Positions::bottomRightFront,Orientation::right,withBlockNums);


	// output block number and color for white face
	output += "\n\n" + leftOffset;
	output += getBlockFaceStr(Positions::topLeftFront,Orientation::front,withBlockNums);
	output += getBlockFaceStr(Positions::topMiddleFront,Orientation::front,withBlockNums);
	output += getBlockFaceStr(Positions::topRightFront,Orientation::front,withBlockNums);

	output += "\n" + leftOffset;
	output += getBlockFaceStr(Positions::middleLeftFront,Orientation::front,withBlockNums);
	output += extraSpace + "W ";
	output += getBlockFaceStr(Positions::middleRightFront,Orientation::front,withBlockNums);

	output += "\n" + leftOffset;
	output += getBlockFaceStr(Positions::bottomLeftFront,Orientation::front,withBlockNums);
	output += getBlockFaceStr(Positions::bottomMiddleFront,Orientation::front,withBlockNums);
	output += getBlockFaceStr(Positions::bottomRightFront,Orientation::front,withBlockNums);


	// output block number and color for green face
	output += "\n\n" + leftOffset;
	output += getBlockFaceStr(Positions::bottomLeftFront,Orientation::bottom,withBlockNums);
	output += getBlockFaceStr(Positions::bottomMiddleFront,Orientation::bottom,withBlockNums);
	output += getBlockFaceStr(Positions::bottomRightFront,Orientation::bottom,withBlockNums);

	output += "\n" + leftOffset;
	output += getBlockFaceStr(Positions::bottomLeftMiddle,Orientation::bottom,withBlockNums);
	output += extraSpace + "G ";
	output += getBlockFaceStr(Positions::bottomRightMiddle,Orientation::bottom,withBlockNums);

	output += "\n" + leftOffset;
	output += getBlockFaceStr(Positions::bottomLeftBack,Orientation::bottom,withBlockNums);
	output += getBlockFaceStr(Positions::bottomMiddleBack,Orientation::bottom,withBlockNums);
	output += getBlockFaceStr(Positions::bottomRightBack,Orientation::bottom,withBlockNums);
	output += "\n\n";

	return output;
}





