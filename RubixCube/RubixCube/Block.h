#pragma once
#include <iostream>
#include <stdexcept>
#include <string>
#include "Enums.h"
#include "Rotator.h"

/****************************************************************
Block is used to store state of an individual block.  It doesn't
have any functions attached to it because I need it to be small
and fast.  The functions to manipulate Block are in BasicBlock
****************************************************************/
struct Block
{
	byte homeBlock;
	orientVector orientationVector;
	Positions position;
};


class BasicBlock
{
private:  
	static bool initialized;
	static rByte normalizer[256];
	static rByte oVector[256];
	static oByte roByteConvert[256];
	void createNormalizers();


	//Positions currentPosition;
	//orientVector currentOrientation;
	Faces faces;

public:
	BasicBlock(void);
	~BasicBlock(void);
	BasicBlock(Positions home,Faces f);
	void initBlock(Positions home, Faces f);
	Positions homePosition;
	orientVector homeOrientation;
	orientVector checkBlock(Faces f);
	Block loadBlock(byte homeBlock,Positions p,orientVector o);
	//bool loaded; // is true if the block has been assigned.  If the BasicBlock is just default initialized
				 // it returns false

	//Rotator currentRotation;  // defaults to homePosition on creation

	bool equalFaces(Faces f);
	static bool equalOrientation(Block b1, Block b2);
	bool isHome(Block b);
	Positions getHome();
	Faces getFaces(orientVector o);

	Faces remapFaces(orientVector selVect);
	Faces addFaceColor(Faces &f, Orientation o, Color c);

	static bool equalBlocks(Block b1, Block b2);




	//rByte rotate(rByte currentVector, rByte offsetVector);
	static void rotate(Block &block,rByte offsetVector); // rotates by the amount in offsetVector
	static rByte deRotate(orientVector oldRotationVector, orientVector newRotationVector);  // reverses a rotation

	static rByte getNRKey(int xRotations,int yRotations, int zRotations);
	static rByte getRKey(int xRotations, int yRotations, int zRotations);
	void getRotations(rByte key, int &xRotations, int &yRotations, int &zRotations);
	static oByte getOKey(Rotator tVector, Rotator fVector);
	static oByte getOKey(rByte RKey);
	orientVector getOrientation(oByte key);
};


