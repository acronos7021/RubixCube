#include "Rotator.h"
#include "Block.h"

const byte Rotator::xAxis[4] = {0,5,1,4}; // top,back,bottom,front
const byte Rotator::yAxis[4] = {4,2,5,3}; // front,left,back,right
const byte Rotator::zAxis[4] = {0,3,1,2}; // top,right,bottom,left

const byte Rotator::rxAxis[6] = {0,2,6,6,3,1};
const byte Rotator::ryAxis[6] = {6,6,1,3,0,2};
const byte Rotator::rzAxis[6] = {0,2,3,1,6,6};
						//       T       F


Rotator::Rotator(void)
{ // load top orientation for default
	ro = Orientation::top;
}


Rotator::Rotator(Orientation o)
{
	ro=o;
}

Rotator::~Rotator(void)
{
}

Rotator::Rotator(byte key)
{
	// the input is a direction that lines up with Orientation
	ro=(Orientation) key;
}	


void Rotator::loadOrientation(Orientation o)
{
	ro=o;
}

// even though it's not really intuitive to return an rByte here, it would
// require Block to do a lot of calculations solve the same thing.
// this only works if the rotation is on a single axis.

//rByte Rotator::difference(Rotator v1, Rotator v2,Rotator axis)
rByte Rotator::difference(Rotator v1,Rotator v2)
{
	byte xAxis=0;
	byte yAxis=0;
	byte zAxis=0;
	if ((rxAxis[(byte) v2.ro] != 6) && (rxAxis[(byte) v1.ro] != 6))
		xAxis=(rxAxis[(byte) v2.ro] + 4 - rxAxis[(byte) v1.ro]) % 4;
	if ((ryAxis[(byte) v2.ro] != 6) && (ryAxis[(byte) v1.ro] != 6))
		yAxis=(ryAxis[(byte) v2.ro] + 4 - ryAxis[(byte) v1.ro]) % 4;
	if ((rzAxis[(byte) v2.ro] != 6) && (rzAxis[(byte) v1.ro] != 6))
		zAxis=(rzAxis[(byte) v2.ro] + 4 - rzAxis[(byte) v1.ro]) % 4;

	// check and correct for opposite vectors.
	if (xAxis==2)
	{
		// the two vectors are opposite each other. That means there are two paths that move between these
		// two vectors.  We return the wrong answer if we return both of them, so pick the xAxis since it is first.
		yAxis=0;
		zAxis=0;
	}
	else if (yAxis==2)
	{
		// opposite vectors and x is already eliminated, so eliminate zAxis
		zAxis=0;
	}
	// else if (zAxis) isn't needed because it will already have been eliminated by one of the previous two choices.
	return BasicBlock::getNRKey(xAxis,yAxis,zAxis);

	//if ((axis.ro==Orientation::top) || (axis.ro==Orientation::bottom))
	//	return ((ryAxis[(byte) v2.ro] + 4 - ryAxis[(byte) v1.ro]) % 4)<<3; // rotate on y axis clockwise
	//else if ((axis.ro==Orientation::front) || (axis.ro==Orientation::back))
	//	return (rzAxis[(byte) v2.ro] + 4 - rzAxis[(byte) v1.ro]) % 4; // rotate on the z axis clockwise
	//else if ((axis.ro==Orientation::right) || (axis.ro==Orientation::left))
	//	return ((rxAxis[(byte) v2.ro] + 4 - rxAxis[(byte) v1.ro]) % 4)<<6; // rotate on the x axis clockwise
	//else
	//	throw std::out_of_range("Cannot calculate the Rotator::difference on this value");


	//if (axis.ro==Orientation::top)
	//	return ((ryAxis[(byte) v2.ro] + 4 - ryAxis[(byte) v1.ro]) % 4)<<3; // rotate on y axis clockwise
	//else if (axis.ro==Orientation::bottom)
	//	return ((ryAxis[(byte) v1.ro] + 4 - ryAxis[(byte) v2.ro]) % 4)<<3; // rotate on y axis counterclockwise
	//else if (axis.ro==Orientation::front)
	//	return (rzAxis[(byte) v2.ro] + 4 - rzAxis[(byte) v1.ro]) % 4; // rotate on the z axis clockwise
	//else if (axis.ro==Orientation::back)
	//	return (rzAxis[(byte) v1.ro] + 4 - rzAxis[(byte) v2.ro]) % 4; // rotate on the z axis counterclockwise
	//else if (axis.ro==Orientation::right)
	//	return ((rxAxis[(byte) v2.ro] + 4 - rxAxis[(byte) v1.ro]) % 4)<<6; // rotate on the x axis clockwise
	//else if (axis.ro==Orientation::left)
	//	return ((rxAxis[(byte) v1.ro] + 4 - rxAxis[(byte) v2.ro]) % 4)<<6; // rotate on the x axis counterclockwise
	//else
	//	throw std::out_of_range("Cannot calculate the Rotator::difference on this value");
}


// this only gets the axis that is clockwise to the two rotators and correct for the positive axis face
// there is probably a better way to do this but I haven't figured it out yet
Rotator Rotator::getAxis(Rotator t,Rotator f)
{
	// determine which axis it is.  It will be the one that isn't in the set
	bool xAxis = true;
	bool yAxis = true;
	bool zAxis = true;

	if ((rxAxis[(byte) t.ro]==6) || (rxAxis[(byte) f.ro]==6)) xAxis=false;
	if ((ryAxis[(byte) t.ro]==6) || (ryAxis[(byte) f.ro]==6)) yAxis=false;
	if ((rzAxis[(byte) t.ro]==6) || (rzAxis[(byte) f.ro]==6)) zAxis=false;

	// the axis that is still true is the correct one.  Since we always rotate on the
	// positive face we don't have to worry about right, bottom, and back.
	if (xAxis) return Rotator(Orientation::right);
	else if (yAxis) return Rotator(Orientation::top);
	else if (zAxis) return Rotator(Orientation::front);
	else throw std::out_of_range("Cannot calculate the axis on this value");
}

bool Rotator::equals(Rotator o)
{
	if (ro==o.ro) return true;
	else return false;
}

bool Rotator::operator ==(Rotator o)
{
	return equals(o);
}

bool Rotator::operator !=(Rotator o)
{
	return !equals(o);
}

// all rotations are clockwise facing their positive axis
// counterclockwise would be 3 rotations on one axis
// x is positive facing right
// y is positive facing top
// z is positive facing front
void Rotator::incXaxis()
{
	if ((ro==Orientation::right) || (ro==Orientation::left)) {} // do nothing
	else if (ro==Orientation::top) ro=Orientation::back;
	else if (ro==Orientation::back) ro=Orientation::bottom;
	else if (ro==Orientation::bottom) ro=Orientation::front;
	else ro=Orientation::top;
}

void Rotator::incYaxis()
{
	if ((ro==Orientation::top) || (ro==Orientation::bottom)) {} // do nothing
	else if (ro==Orientation::front) ro=Orientation::left;
	else if (ro==Orientation::left) ro=Orientation::back;
	else if (ro==Orientation::back) ro=Orientation::right;
	else ro=Orientation::front;
}

void Rotator::incZaxis()
{
	if ((ro==Orientation::front) || (ro==Orientation::back)) {} // do nothing
	else if (ro==Orientation::top) ro=Orientation::right;
	else if (ro==Orientation::right) ro=Orientation::bottom;
	else if (ro==Orientation::bottom) ro=Orientation::left;
	else ro=Orientation::top;
}

//
//void Rotator::decXaxis()
//{
//	if ((ro==Orientation::right) || (ro==Orientation::left)) {} // do nothing
//	else if (ro==Orientation::top) ro=Orientation::front;
//	else if (ro==Orientation::front) ro=Orientation::bottom;
//	else if (ro==Orientation::bottom) ro=Orientation::back;
//	else ro=Orientation::top;
//}
//
//void Rotator::decYaxis()
//{
//	if ((ro==Orientation::top) || (ro==Orientation::bottom)) {} // do nothing
//	else if (ro==Orientation::front) ro=Orientation::right;
//	else if (ro==Orientation::right) ro=Orientation::back;
//	else if (ro==Orientation::back) ro=Orientation::left;
//	else ro=Orientation::front;
//}
//
//void Rotator::decZaxis()
//{
//	if ((ro==Orientation::front) || (ro==Orientation::back)) {} // do nothing
//	else if (ro==Orientation::top) ro=Orientation::left;
//	else if (ro==Orientation::left) ro=Orientation::bottom;
//	else if (ro==Orientation::bottom) ro=Orientation::right;
//	else ro=Orientation::top;
//}



Rotator Rotator::getOpposite()
{
	// there are 6 possibilities
	if (ro==Orientation::top) return Rotator(Orientation::bottom);
	else if (ro==Orientation::bottom) return Rotator(Orientation::top);
	else if (ro==Orientation::front) return Rotator(Orientation::back);
	else if (ro==Orientation::back) return Rotator(Orientation::front);
	else if (ro==Orientation::left) return Rotator(Orientation::right);
	else return Rotator(Orientation::left);
}

Orientation Rotator::getOrientation()
{
	return ro;
}

std::string Rotator::toString()
{
	if (ro==Orientation::top)		return "top";
	if (ro==Orientation::bottom)	return "bottom";
	if (ro==Orientation::left)	return "left";
	if (ro==Orientation::right)	return "right";
	if (ro==Orientation::front)	return "front";
	if (ro==Orientation::back)	return "back";
	return "invalid";
}
