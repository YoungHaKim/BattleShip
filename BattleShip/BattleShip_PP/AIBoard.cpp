#include "stdafx.h"
#include "AIBoard.h"
#include "Board.h"




AIBoard::~AIBoard()
{
}


void AIBoard::MarkAsHit(Coordinate coordinate)
{
	if (MapBoundaryCheck(coordinate.x, coordinate.y))
	{
		m_Board[coordinate.x][coordinate.y] = 1;
	}
}
void AIBoard::MarkAsMiss(Coordinate coordinate)
{
	if (MapBoundaryCheck(coordinate.x, coordinate.y))
	{
		m_Board[coordinate.x][coordinate.y] = -1;
	}
}
bool AIBoard::IsNextAttackValid(Coordinate coordinate)
{
	if (MapBoundaryCheck(coordinate.x, coordinate.y))
	{
		if (m_Board[coordinate.x][coordinate.y] == 0)
			return true;
	}

	return false;
}