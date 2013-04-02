#include <sstream>
#include <vector>
#include <algorithm>  
#include <iostream>
#include <time.h>
#include "Cube.h"

// makes the static variable "initalized" be set to false
// the first time Cube is instantiated.
bool Cube::initialized = false;
BasicBlock Cube::homeBlocks[20];
std::vector<TransferFunction> Cube::transferFunctions;
std::map<TransferMoveSet,int> Cube::reverseTransferFunctionMap;

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
	rByte key = BasicBlock::getNRKey(0,1,0);
	std::vector<TransferMoves> tcw;
	tcw.push_back(createTransferMove(Positions::topLeftFront,Positions::topLeftBack,key));
	tcw.push_back(createTransferMove(Positions::topLeftBack,Positions::topRightBack,key));
	tcw.push_back(createTransferMove(Positions::topRightBack,Positions::topRightFront,key));
	tcw.push_back(createTransferMove(Positions::topRightFront,Positions::topLeftFront,key));
	tcw.push_back(createTransferMove(Positions::topLeftMiddle,Positions::topMiddleBack,key));
	tcw.push_back(createTransferMove(Positions::topMiddleBack,Positions::topRightMiddle,key));
	tcw.push_back(createTransferMove(Positions::topRightMiddle,Positions::topMiddleFront,key));
	tcw.push_back(createTransferMove(Positions::topMiddleFront,Positions::topLeftMiddle,key));
	std::sort(tcw.begin(),tcw.end()); //reorder the list so that it matches the automated one
	TransferMoveSet tcwms = TransferMoveSet(tcw);
	reverseTransferFunctionMap.insert(reverseTransferFunctionPair(tcwms,0));
	transferFunctions.push_back(TransferFunction(Moves::topClockwise,tcwms));
	
	// topCounterClockwise
	key = BasicBlock::getNRKey(0,3,0);
	std::vector<TransferMoves> tcc;
	tcc.push_back(createTransferMove(Positions::topLeftFront,Positions::topRightFront,key));
	tcc.push_back(createTransferMove(Positions::topRightFront,Positions::topRightBack,key));
	tcc.push_back(createTransferMove(Positions::topRightBack,Positions::topLeftBack,key));
	tcc.push_back(createTransferMove(Positions::topLeftBack,Positions::topLeftFront,key));
	tcc.push_back(createTransferMove(Positions::topLeftMiddle,Positions::topMiddleFront,key));
	tcc.push_back(createTransferMove(Positions::topMiddleFront,Positions::topRightMiddle,key));
	tcc.push_back(createTransferMove(Positions::topRightMiddle,Positions::topMiddleBack,key));
	tcc.push_back(createTransferMove(Positions::topMiddleBack,Positions::topLeftMiddle,key));
	std::sort(tcc.begin(),tcc.end()); //reorder the list so that it matches the automated one
	TransferMoveSet tccms = TransferMoveSet(tcc);
	reverseTransferFunctionMap.insert(reverseTransferFunctionPair(tccms,1));
	transferFunctions.push_back(TransferFunction(Moves::topCounterClockwise,tccms));


	//bottomClockwise
	key = BasicBlock::getNRKey(0,3,0);
	std::vector<TransferMoves> bcc;
	bcc.push_back(createTransferMove(Positions::bottomLeftFront,Positions::bottomRightFront,key));
	bcc.push_back(createTransferMove(Positions::bottomRightFront,Positions::bottomRightBack,key));
	bcc.push_back(createTransferMove(Positions::bottomRightBack,Positions::bottomLeftBack,key));
	bcc.push_back(createTransferMove(Positions::bottomLeftBack,Positions::bottomLeftFront,key));
	bcc.push_back(createTransferMove(Positions::bottomLeftMiddle,Positions::bottomMiddleFront,key));
	bcc.push_back(createTransferMove(Positions::bottomMiddleFront,Positions::bottomRightMiddle,key));
	bcc.push_back(createTransferMove(Positions::bottomRightMiddle,Positions::bottomMiddleBack,key));
	bcc.push_back(createTransferMove(Positions::bottomMiddleBack,Positions::bottomLeftMiddle,key));
	std::sort(bcc.begin(),bcc.end()); //reorder the list so that it matches the automated one
	TransferMoveSet bccms = TransferMoveSet(bcc);
	reverseTransferFunctionMap.insert(reverseTransferFunctionPair(bccms,2));
	transferFunctions.push_back(TransferFunction(Moves::bottomClockwise,bccms));

	//bottomCounterClockwise
	key = BasicBlock::getNRKey(0,1,0);
	std::vector<TransferMoves> bcw;
	bcw.push_back(createTransferMove(Positions::bottomLeftFront,Positions::bottomLeftBack,key));
	bcw.push_back(createTransferMove(Positions::bottomLeftBack,Positions::bottomRightBack,key));
	bcw.push_back(createTransferMove(Positions::bottomRightBack,Positions::bottomRightFront,key));
	bcw.push_back(createTransferMove(Positions::bottomRightFront,Positions::bottomLeftFront,key));
	bcw.push_back(createTransferMove(Positions::bottomLeftMiddle,Positions::bottomMiddleBack,key));
	bcw.push_back(createTransferMove(Positions::bottomMiddleBack,Positions::bottomRightMiddle,key));
	bcw.push_back(createTransferMove(Positions::bottomRightMiddle,Positions::bottomMiddleFront,key));
	bcw.push_back(createTransferMove(Positions::bottomMiddleFront,Positions::bottomLeftMiddle,key));
	std::sort(bcw.begin(),bcw.end()); //reorder the list so that it matches the automated one
	TransferMoveSet bcwms = TransferMoveSet(bcw);
	reverseTransferFunctionMap.insert(reverseTransferFunctionPair(bcwms,3));	
	transferFunctions.push_back(TransferFunction(Moves::bottomCounterClockwise,bcwms));

	//rightClockwise
//	transferFunctions.push_back(rightClockwise);
	key = BasicBlock::getNRKey(1,0,0);
	std::vector<TransferMoves> rcw;
	rcw.push_back(createTransferMove(Positions::topRightFront,Positions::topRightBack,key));
	rcw.push_back(createTransferMove(Positions::topRightBack,Positions::bottomRightBack,key));
	rcw.push_back(createTransferMove(Positions::bottomRightBack,Positions::bottomRightFront,key));
	rcw.push_back(createTransferMove(Positions::bottomRightFront,Positions::topRightFront,key));
	rcw.push_back(createTransferMove(Positions::topRightMiddle,Positions::middleRightBack,key));
	rcw.push_back(createTransferMove(Positions::middleRightBack,Positions::bottomRightMiddle,key));
	rcw.push_back(createTransferMove(Positions::bottomRightMiddle,Positions::middleRightFront,key));
	rcw.push_back(createTransferMove(Positions::middleRightFront,Positions::topRightMiddle,key));
	std::sort(rcw.begin(),rcw.end()); //reorder the list so that it matches the automated one
	TransferMoveSet rcwms = TransferMoveSet(rcw);
	reverseTransferFunctionMap.insert(reverseTransferFunctionPair(rcwms,4));	
	transferFunctions.push_back(TransferFunction(Moves::rightClockwise,rcwms));

	//rightCounterClockwise
	key = BasicBlock::getNRKey(3,0,0);
	std::vector<TransferMoves> rcc;
	rcc.push_back(createTransferMove(Positions::topRightFront,Positions::bottomRightFront,key));
	rcc.push_back(createTransferMove(Positions::bottomRightFront,Positions::bottomRightBack,key));
	rcc.push_back(createTransferMove(Positions::bottomRightBack,Positions::topRightBack,key));
	rcc.push_back(createTransferMove(Positions::topRightBack,Positions::topRightFront,key));
	rcc.push_back(createTransferMove(Positions::topRightMiddle,Positions::middleRightFront,key));
	rcc.push_back(createTransferMove(Positions::middleRightFront,Positions::bottomRightMiddle,key));
	rcc.push_back(createTransferMove(Positions::bottomRightMiddle,Positions::middleRightBack,key));
	rcc.push_back(createTransferMove(Positions::middleRightBack,Positions::topRightMiddle,key));
	std::sort(rcc.begin(),rcc.end()); //reorder the list so that it matches the automated one
	TransferMoveSet rccms = TransferMoveSet(rcc);
	reverseTransferFunctionMap.insert(reverseTransferFunctionPair(rccms,5));	
	transferFunctions.push_back(TransferFunction(Moves::rightCounterClockwise,rccms));

	//leftClockwise
	key = BasicBlock::getNRKey(3,0,0);
	std::vector<TransferMoves> lcw;
	lcw.push_back(createTransferMove(Positions::topLeftFront,Positions::bottomLeftFront,key));
	lcw.push_back(createTransferMove(Positions::bottomLeftFront,Positions::bottomLeftBack,key));
	lcw.push_back(createTransferMove(Positions::bottomLeftBack,Positions::topLeftBack,key));
	lcw.push_back(createTransferMove(Positions::topLeftBack,Positions::topLeftFront,key));
	lcw.push_back(createTransferMove(Positions::topLeftMiddle,Positions::middleLeftFront,key));
	lcw.push_back(createTransferMove(Positions::middleLeftFront,Positions::bottomLeftMiddle,key));
	lcw.push_back(createTransferMove(Positions::bottomLeftMiddle,Positions::middleLeftBack,key));
	lcw.push_back(createTransferMove(Positions::middleLeftBack,Positions::topLeftMiddle,key));
	std::sort(lcw.begin(),lcw.end()); //reorder the list so that it matches the automated one
	TransferMoveSet lcwms = TransferMoveSet(lcw);
	reverseTransferFunctionMap.insert(reverseTransferFunctionPair(lcwms,6));	
	transferFunctions.push_back(TransferFunction(Moves::leftClockwise,lcwms));

	//leftCounterClockwise
	key = BasicBlock::getNRKey(1,0,0);
	std::vector<TransferMoves> lcc;
	lcc.push_back(createTransferMove(Positions::topLeftFront,Positions::topLeftBack,key));
	lcc.push_back(createTransferMove(Positions::topLeftBack,Positions::bottomLeftBack,key));
	lcc.push_back(createTransferMove(Positions::bottomLeftBack,Positions::bottomLeftFront,key));
	lcc.push_back(createTransferMove(Positions::bottomLeftFront,Positions::topLeftFront,key));
	lcc.push_back(createTransferMove(Positions::topLeftMiddle,Positions::middleLeftBack,key));
	lcc.push_back(createTransferMove(Positions::middleLeftBack,Positions::bottomLeftMiddle,key));
	lcc.push_back(createTransferMove(Positions::bottomLeftMiddle,Positions::middleLeftFront,key));
	lcc.push_back(createTransferMove(Positions::middleLeftFront,Positions::topLeftMiddle,key));
	std::sort(lcc.begin(),lcc.end()); //reorder the list so that it matches the automated one
	TransferMoveSet lccms = TransferMoveSet(lcc);
	reverseTransferFunctionMap.insert(reverseTransferFunctionPair(lccms,7));	
	transferFunctions.push_back(TransferFunction(Moves::leftCounterClockwise,lccms));

	//frontClockwise
	key = BasicBlock::getNRKey(0,0,1);
	std::vector<TransferMoves> fcw;
	fcw.push_back(createTransferMove(Positions::topLeftFront,Positions::topRightFront,key));
	fcw.push_back(createTransferMove(Positions::topRightFront,Positions::bottomRightFront,key));
	fcw.push_back(createTransferMove(Positions::bottomRightFront,Positions::bottomLeftFront,key));
	fcw.push_back(createTransferMove(Positions::bottomLeftFront,Positions::topLeftFront,key));
	fcw.push_back(createTransferMove(Positions::topMiddleFront,Positions::middleRightFront,key));
	fcw.push_back(createTransferMove(Positions::middleRightFront,Positions::bottomMiddleFront,key));
	fcw.push_back(createTransferMove(Positions::bottomMiddleFront,Positions::middleLeftFront,key));
	fcw.push_back(createTransferMove(Positions::middleLeftFront,Positions::topMiddleFront,key));
	std::sort(fcw.begin(),fcw.end()); //reorder the list so that it matches the automated one
	TransferMoveSet fcwms = TransferMoveSet(fcw);
	reverseTransferFunctionMap.insert(reverseTransferFunctionPair(fcwms,8));
	transferFunctions.push_back(TransferFunction(Moves::frontClockwise,fcwms));


	//frontCounterClockwise
	key = BasicBlock::getNRKey(0,0,3);
	std::vector<TransferMoves> fcc;
	fcc.push_back(createTransferMove(Positions::topLeftFront,Positions::bottomLeftFront,key));
	fcc.push_back(createTransferMove(Positions::bottomLeftFront,Positions::bottomRightFront,key));
	fcc.push_back(createTransferMove(Positions::bottomRightFront,Positions::topRightFront,key));
	fcc.push_back(createTransferMove(Positions::topRightFront,Positions::topLeftFront,key));
	fcc.push_back(createTransferMove(Positions::topMiddleFront,Positions::middleLeftFront,key));
	fcc.push_back(createTransferMove(Positions::middleLeftFront,Positions::bottomMiddleFront,key));
	fcc.push_back(createTransferMove(Positions::bottomMiddleFront,Positions::middleRightFront,key));
	fcc.push_back(createTransferMove(Positions::middleRightFront,Positions::topMiddleFront,key));
	std::sort(fcc.begin(),fcc.end()); //reorder the list so that it matches the automated one
	TransferMoveSet fccms = TransferMoveSet(fcc);
	reverseTransferFunctionMap.insert(reverseTransferFunctionPair(fccms,9));	
	transferFunctions.push_back(TransferFunction(Moves::frontCounterClockwise,fccms));


	//backClockwise
	key = BasicBlock::getNRKey(0,0,3);
	std::vector<TransferMoves> bacw;
	bacw.push_back(createTransferMove(Positions::topLeftBack,Positions::bottomLeftBack,key));
	bacw.push_back(createTransferMove(Positions::bottomLeftBack,Positions::bottomRightBack,key));
	bacw.push_back(createTransferMove(Positions::bottomRightBack,Positions::topRightBack,key));
	bacw.push_back(createTransferMove(Positions::topRightBack,Positions::topLeftBack,key));
	bacw.push_back(createTransferMove(Positions::topMiddleBack,Positions::middleLeftBack,key));
	bacw.push_back(createTransferMove(Positions::middleLeftBack,Positions::bottomMiddleBack,key));
	bacw.push_back(createTransferMove(Positions::bottomMiddleBack,Positions::middleRightBack,key));
	bacw.push_back(createTransferMove(Positions::middleRightBack,Positions::topMiddleBack,key));
	std::sort(bacw.begin(),bacw.end()); //reorder the list so that it matches the automated one
	TransferMoveSet bacwms = TransferMoveSet(bacw);
	reverseTransferFunctionMap.insert(reverseTransferFunctionPair(bacwms,10));	
	transferFunctions.push_back(TransferFunction(Moves::backClockwise,bacwms));

	//backCounterClockwise
	key = BasicBlock::getNRKey(0,0,1);
	std::vector<TransferMoves> bacc;
	bacc.push_back(createTransferMove(Positions::topLeftBack,Positions::topRightBack,key));
	bacc.push_back(createTransferMove(Positions::topRightBack,Positions::bottomRightBack,key));
	bacc.push_back(createTransferMove(Positions::bottomRightBack,Positions::bottomLeftBack,key));
	bacc.push_back(createTransferMove(Positions::bottomLeftBack,Positions::topLeftBack,key));
	bacc.push_back(createTransferMove(Positions::topMiddleBack,Positions::middleRightBack,key));
	bacc.push_back(createTransferMove(Positions::middleRightBack,Positions::bottomMiddleBack,key));
	bacc.push_back(createTransferMove(Positions::bottomMiddleBack,Positions::middleLeftBack,key));
	bacc.push_back(createTransferMove(Positions::middleLeftBack,Positions::topMiddleBack,key));
	std::sort(bacc.begin(),bacc.end()); //reorder the list so that it matches the automated one
	TransferMoveSet baccms = TransferMoveSet(bacc);
	reverseTransferFunctionMap.insert(reverseTransferFunctionPair(baccms,11));	
	transferFunctions.push_back(TransferFunction(Moves::backCounterClockwise,baccms));
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
		endGenerationIndex = transferFunctions.size();  
		for (size_t tfIndex=beginGenerationIndex; tfIndex < endGenerationIndex; ++tfIndex)
		{
			// only work on the last generation.  Otherwise, we duplicate all the previous generations again
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
					TransferMoveSet newTransferMoveSet = TransferMoveSet(newBlockMoves);
					std::pair<std::map<TransferMoveSet,int>::iterator,bool> 
						ret = reverseTransferFunctionMap.insert(reverseTransferFunctionPair(newTransferMoveSet,transferFunctions.size()));

					if (ret.second)
					{
						// the insertion succeeded so the function was not already in the database.
						// add the new transfer function.
	
						std::vector<Moves> newTFface;
						//newTF.transferBlockMoves = newBlockMoves;
						// add the face moves that were already calculated
						for (size_t i=0; i < transferFunctions[tfIndex].transferFaceMoves.size(); ++i)
						{
							newTFface.push_back(transferFunctions[tfIndex].transferFaceMoves[i]);
						}
						// add the new face move
						newTFface.push_back((Moves)basicIndex);
						TransferFunction newTF(newTFface,newTransferMoveSet);
						transferFunctions.push_back(newTF);
						totalSize+= sizeof(TransferMoveSet);
						totalSize+= sizeof(TransferMoves) * newTransferMoveSet.getSize();
						totalSize+= sizeof(Moves) * newTF.transferFaceMoves.size();
						lastGenerationAdditions++;
						//if (lastGenerationAdditions%50 == 15)
						//{
							std::cout << std::endl;
							int lastTransferIndex = transferFunctions.size()-1;
							std::cout << showMoves(lastTransferIndex);
							testCube.SetCubeToSolved();
							testCube.move(lastTransferIndex);
							std::cout << std::endl;
							std::cout << testCube.toString(false);
						//}
					}
					else
					{
						++alreadyExistsCount;
					}
				}
			}
		}
		beginGenerationIndex=endGenerationIndex;
		endTimer=time(nullptr);
		int seconds = (int) difftime(endTimer, startTimer);
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
void Cube::move(size_t ti)
{
	Cube oldCube(this); // load a copy of this cube into old cube
	for (size_t i=0;i<transferFunctions[(byte)ti].transferBlockMoves.getSize();++i)
	{
		// get the old block
		TransferMoves* newMove = transferFunctions[(byte)ti].transferBlockMoves.getMoves();
		int oldBlock = (byte)newMove[i].oldPosition;
		int newBlock = (byte) newMove[i].newPosition;
		blocks2[newBlock] = oldCube.blocks2[oldBlock];
		BasicBlock::rotate(blocks2[newBlock],newMove[i].rotate);
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
/*
enum class Moves : unsigned char {topClockwise,topCounterClockwise,
									bottomClockwise,bottomCounterClockwise,
									rightClockwise,rightCounterClockwise,
									leftClockwise,leftCounterClockwise,
									frontClockwise,frontCounterClockwise,
									backClockwise,backCounterClockwise};*/

// prints out the moves for that transfer function
std::string Cube::showMoves(size_t ti)
{
	std::string output = "";
	size_t lastTRfunction=transferFunctions[(byte)ti].transferFaceMoves.size();
	for (size_t i=0;i<lastTRfunction;++i)
	{
		if (transferFunctions[(byte)ti].transferFaceMoves[i] == Moves::topClockwise) output += "T";
		if (transferFunctions[(byte)ti].transferFaceMoves[i] == Moves::topCounterClockwise) output +=  "T'";
		if (transferFunctions[(byte)ti].transferFaceMoves[i] == Moves::bottomClockwise) output +=  "B";
		if (transferFunctions[(byte)ti].transferFaceMoves[i] == Moves::bottomCounterClockwise) output +=  "B'";
		if (transferFunctions[(byte)ti].transferFaceMoves[i] == Moves::rightClockwise) output +=  "R";
		if (transferFunctions[(byte)ti].transferFaceMoves[i] == Moves::rightCounterClockwise) output +=  "R'";
		if (transferFunctions[(byte)ti].transferFaceMoves[i] == Moves::leftClockwise) output +=  "L";
		if (transferFunctions[(byte)ti].transferFaceMoves[i] == Moves::leftCounterClockwise) output +=  "L'";
		if (transferFunctions[(byte)ti].transferFaceMoves[i] == Moves::frontClockwise) output +=  "F";
		if (transferFunctions[(byte)ti].transferFaceMoves[i] == Moves::frontCounterClockwise) output +=  "F'";
		if (transferFunctions[(byte)ti].transferFaceMoves[i] == Moves::backClockwise) output +=  "Ba";
		if (transferFunctions[(byte)ti].transferFaceMoves[i] == Moves::backCounterClockwise) output +=  "Ba'";
		if (i!=lastTRfunction-1) output +=  ",";
	}
	return output;
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





