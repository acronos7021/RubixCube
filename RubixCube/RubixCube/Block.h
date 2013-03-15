#pragma once

#include "Enums.h"

class Block
{
private:  // make sure the block is created with the proper constructor
	Block(void);

public:
	~Block(void);
	Block(Positions home,Color to,Color bo, Color le, Color ri, Color fr, Color ba);

	Positions homePosition;	
	Color top,bottom,left,right,front,back;

	// returns invalid if there is no orientation that matches the color pattern.
	bool getOrientation(Color to,Color bo, Color le, Color ri, Color fr, Color ba);
};

