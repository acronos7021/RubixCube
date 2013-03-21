#pragma once
#include <iostream>
#include <stdexcept>
#include <string>
#include "Enums.h"
#include "Rotator.h"




class Block
{
private:  // make sure the block is created with the proper constructor
	static bool initialized;
	static rByte normalizer[256];
	static oByte oVector[256];
	static roByte roByteConvert[256];
	void createNormalizers();

	Positions homePosition;
	orientVector homeOrientation;
	Positions currentPosition;
	orientVector currentOrientation;
	Faces faces;


public:
	Block(void);
	~Block(void);
	Block(Positions home,Faces f);
	void initBlock(Positions home, Faces f);
	bool loadBlock(Positions currPos, Faces f);
	bool loaded; // is true if the block has been assigned.  If the block is just default initialized
				 // it returns false

	//Rotator currentRotation;  // defaults to homePosition on creation

	bool equalFaces(Faces f);
	bool equalOrientation(Block in);
	bool isHome();


	rByte rotate(rByte currentVector, rByte offsetVector);
	rByte getRKey(int xRotations, int yRotations, int zRotations);
	void getRotations(rByte key, int &xRotations, int &yRotations, int &zRotations);
	oByte getOKey(Rotator tVector, Rotator fVector);
	oByte getOKey(rByte RKey);
	orientVector getOrientation(oByte key);
};


