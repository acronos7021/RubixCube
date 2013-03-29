#include "Rotator.h"


Rotator::Rotator(void)
{ // load top orientation for default
	x=0;
	y=1;
	z=0;
	//color=none;
}

Rotator::Rotator(char xi, char yi, char zi)
{
	if (getOrientation(xi,yi,zi)==Orientation::invalid) 
		throw std::out_of_range("there is no orientation that matches");
	x=xi;
	y=yi;
	z=zi;
	//color = none;
}

//Rotator::Rotator(char xi,char yi, char zi, Color c)
//{
//	if (getOrientation(xi,yi,zi)==invalid) 
//		throw std::out_of_range("there is no orientation that matches");
//	x=xi;
//	y=yi;
//	z=zi;
//	color = c;
//}

Rotator::Rotator(Orientation o)
{
	loadOrientation(o);
}

//Rotator::Rotator(Orientation o, Color c)
//{
//	loadOrientation(o);
//	color = c;
//}

Rotator::~Rotator(void)
{
}

Rotator::Rotator(byte key)
{
	// the input is a direction that lines up with Orientation
	Orientation o=(Orientation) key;
	loadOrientation(o);
}	


void Rotator::loadOrientation(Orientation o)
{
	if      (o==Orientation::top)	{x=0;y=1;z=0;}
	else if (o==Orientation::bottom)	{x=0;y=-1;z=0;}
	else if (o==Orientation::left)	{x=-1;y=0;z=0;}
	else if (o==Orientation::right)	{x=1;y=0;z=0;}
	else if (o==Orientation::front)	{x=0;y=0;z=1;}
	else if (o==Orientation::back)	{x=0;y=0;z=-1;}
}

// all rotations are clockwise facing their positive axis
// counterclockwise would be 3 rotations on one axis
// x is positive facing right
// y is positive facing top
// z is positive facing front
void Rotator::incXaxis()
{
	if ((x==1)||(x==-1)) {}//do nothing
	else if ((y == 1)&&(z == 0)) {y= 0;z=-1;}
	else if ((y == 0)&&(z ==-1)) {y=-1;z= 0;}
	else if ((y ==-1)&&(z == 0)) {y= 0;z= 1;}
	else if ((y == 0)&&(z == 1)) {y= 1;z= 0;}
}

void Rotator::incYaxis()
{
	if ((y==1)||(y==-1)) {}//do nothing
	else if ((x == 1)&&(z == 0)) {x= 0;z= 1;}
	else if ((x == 0)&&(z == 1)) {x=-1;z= 0;}
	else if ((x ==-1)&&(z == 0)) {x= 0;z=-1;}
	else if ((x == 0)&&(z ==-1)) {x= 1;z= 0;}
}

void Rotator::incZaxis()
{
	if ((z==1)||(z==-1)) {}//do nothing
	else if ((x == 0)&&(y == 1)) {x= 1;y= 0;}
	else if ((x == 1)&&(y == 0)) {x= 0;y=-1;}
	else if ((x == 0)&&(y ==-1)) {x=-1;y= 0;}
	else if ((x ==-1)&&(y == 0)) {x= 0;y= 1;}
}

void Rotator::decXaxis()
{
	if ((x==1)||(x==-1)) {}//do nothing
	else if ((y == 1)&&(z == 0)) {y= 0;z= 1;}
	else if ((y == 0)&&(z == 1)) {y=-1;z= 0;}
	else if ((y ==-1)&&(z == 0)) {y= 0;z=-1;}
	else if ((y == 0)&&(z ==-1)) {y= 1;z= 0;}
}

void Rotator::decYaxis()
{
	if ((y==1)||(y==-1)) {}//do nothing
	else if ((x == 1)&&(z == 0)) {x= 0;z=-1;}
	else if ((x == 0)&&(z ==-1)) {x=-1;z= 0;}
	else if ((x ==-1)&&(z == 0)) {x= 0;z= 1;}
	else if ((x == 0)&&(z == 1)) {x= 1;z= 0;}
}

void Rotator::decZaxis()
{
	if ((z==1)||(z==-1)) {}//do nothing
	else if ((x == 0)&&(y == 1)) {x=-1;y= 0;}
	else if ((x ==-1)&&(y == 0)) {x= 0;y=-1;}
	else if ((x == 0)&&(y ==-1)) {x= 1;y= 0;}
	else if ((x == 1)&&(y == 0)) {x= 0;y= 1;}
}


bool Rotator::equals(Rotator o)
{
	if ((x == o.x) && (y == o.y) && (z == o.z))
		return true;
	else
		return false;
}

bool Rotator::operator ==(Rotator o)
{
	return equals(o);
}

Orientation Rotator::getOrientation()
{
	return getOrientation(x,y,z);
}

Rotator Rotator::getOpposite()
{
	if (x!=0) return Rotator(-x,y,z);
	else if (y!=0) return Rotator(x,-y,z);
	else if (z!=0) return Rotator(x,y,-z);
	return (0,0,0); // should never happen
}

Orientation Rotator::getOrientation(int xi, int yi, int zi)
{
	if ((xi== 0) && (yi== 1) && (zi== 0)) return Orientation::top;
	if ((xi== 0) && (yi==-1) && (zi== 0)) return Orientation::bottom;
	if ((xi==-1) && (yi== 0) && (zi== 0)) return Orientation::left;
	if ((xi== 1) && (yi== 0) && (zi== 0)) return Orientation::right;
	if ((xi== 0) && (yi== 0) && (zi== 1)) return Orientation::front;
	if ((xi== 0) && (yi== 0) && (zi==-1)) return Orientation::back;
	return Orientation::invalid;
}

std::string Rotator::toString()
{
	Orientation o = getOrientation();
	if (o==Orientation::top)		return "top";
	if (o==Orientation::bottom)	return "bottom";
	if (o==Orientation::left)	return "left";
	if (o==Orientation::right)	return "right";
	if (o==Orientation::front)	return "front";
	if (o==Orientation::back)	return "back";
	return "invalid";
}




