#pragma once
#include "Enums.h"
//class TransferMoves
//{
//public:
//	TransferMoves(void);
//	~TransferMoves(void);
//};


class TransferMoves
{
public:
	Positions oldPosition;
	Positions newPosition;
	rByte rotate;

	const bool operator==(const TransferMoves tm)
	{
		return ((oldPosition==tm.oldPosition) 
			&& (newPosition==tm.newPosition) 
			&& (rotate==tm.rotate));
	}

	friend bool operator< (const TransferMoves& tm1, const TransferMoves& tm2)
	{
		// sort order : oldPosition, newPosition, rotate
		if (tm1.oldPosition==tm2.oldPosition)
		{
			if (tm1.newPosition==tm2.newPosition)
			{
				if (tm1.rotate==tm2.rotate)
					return false;  // tm1 and tm2 are equal
				else
					return tm1.rotate < tm2.rotate;
			}
			else
				return tm1.newPosition < tm2.newPosition;
		}
		else
			return tm1.oldPosition < tm2.oldPosition;
	}

	void operator=(const TransferMoves tm)
	{
		oldPosition=tm.oldPosition;
		newPosition=tm.newPosition;
		rotate = tm.rotate;
	}
};

bool operator<(const TransferMoves& tm1, const TransferMoves& tm2);



class TransferMoveSet
{
	TransferMoves moves[20];
	char size;
public:
	// assumes that vtm is already sorted. 
	TransferMoveSet(std::vector<TransferMoves> vtm)
	{
		size = vtm.size();
		if (size>0)
		{
//			moves = new TransferMoves[size];
			for (byte i=0;i<vtm.size();++i)
			{
				moves[i]=vtm[i];
			}
		}
		else
		{
//			moves = nullptr;
			size=-1;
		}
	}

	TransferMoveSet()
	{
		size=-1;
//		moves=nullptr;
	}

	TransferMoveSet(const TransferMoveSet &tms)
	{
		copy(tms);
	}


	TransferMoveSet& operator=( const TransferMoveSet tms)
	{
		copy(tms);
		return *this;
	}

	void copy(const TransferMoveSet &tms)
	{
		size=tms.size;
		// copy the moves
//		moves = new TransferMoves[size];
		for (int i=0;i<size;++i)
			moves[i]=tms.moves[i];
	}

	byte getSize()
	{
		return size;
	}

	TransferMoves* getMoves()
	{
		return moves;
	}

	void clear()
	{
//		if (size!=-1)
//			delete moves;
		size=-1;
//		moves=nullptr;
	}

	// this creates a new TransferMoveSet with a new move without deleting the old one.
	TransferMoveSet addMove(TransferMoves newMove)
	{
		TransferMoveSet newSet;
		newSet.size = size+1;
//		newSet.moves = new TransferMoves[newSet.size];
		byte i;
		if (size>0)
		{
			for (i=0;i<size;++i)
			{
				newSet.moves[i]=moves[i];
			}
		}
		newSet.moves[i]=newMove;
		return newSet;
	}

	std::vector<TransferMoves> getSet()
	{
		std::vector<TransferMoves> vtm;
		for (byte i=0;i<size;++i)
			vtm.push_back(moves[i]);
		return vtm;
	}

	bool operator==(const TransferMoveSet tms)
	{
		for (int i=0;i<size;++i)
			if (!(moves[i] == tms.moves[i]))
				return false;
		// if we got here they are all the same
		return true;
	}

	friend bool operator<(const TransferMoveSet& tms1, const TransferMoveSet& tms2)
	{
		if (tms1.size<tms2.size)
			return true;
		else if (tms1.size>tms2.size)
			return false;
		else
		{
			// they are equal so we have to compare each
			for (int i=0;i<tms1.size;++i)
			{
				TransferMoves tm = tms1.moves[i];
				if (tm==tms2.moves[i])
				{ // go to the next item, do nothing.
				}
				else if (tms1.moves[i]<tms2.moves[i])
					return true;
				else 
					return false;
				
			}
		}
		// if we got here, they are equal so return false;
		return false;
	}

	~TransferMoveSet()
	{
		clear();
	}
};

bool operator<(const TransferMoveSet& tms1, const TransferMoveSet& tms2);

