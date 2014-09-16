#pragma once
#include "Board.h"
#include "Enums.h"

class AIBoard :
	public Board
{
public:
	AIBoard(int width, int height) : Board(width, height)
	{

	}
	~AIBoard();

	void		MarkAIBoardWithSunkenShip(ShipType shipType, Coordinate lastAttackCoord);
	void		MarkAsHit(Coordinate coordinate);
	void		MarkAsMiss(Coordinate coordinate);
	bool		IsNextAttackValid(Coordinate coordinate);
	bool		CheckIfSunkAndMark(int x, int y, int shipSize, Direction direction);
	bool		CheckForHitButUnsunkShips();
	Coordinate	GetUnsunkHitCoordinate();

private:
	void MarkAsSunk(int x, int y, int shipSize);
};

