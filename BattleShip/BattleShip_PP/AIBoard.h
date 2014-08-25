#pragma once
#include "Board.h"

class AIBoard :
	public Board
{
public:
	AIBoard(int width, int height) : Board(width, height)
	{

	}
	~AIBoard();

	void MarkAsHit(Coordinate coordinate);
	void MarkAsMiss(Coordinate coordinate);
	bool IsNextAttackValid(Coordinate coordinate);
};

