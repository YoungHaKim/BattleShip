#include "stdafx.h"
#include "AIBoard.h"
#include "Board.h"
#include "Ship.h"



AIBoard::~AIBoard()
{
}


void AIBoard::MarkAsHit(Coordinate coordinate)
{
	if (MapBoundaryCheck(coordinate.x, coordinate.y))
	{
		if (m_Board[coordinate.x][coordinate.y] == 0)
			m_Board[coordinate.x][coordinate.y] = 1;
		/*else
			printf_s("ERROR! trying to mark non empty square as hit!\n");*/
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

void AIBoard::MarkAsSunk(int x, int y, int shipSize)
{
	if (MapBoundaryCheck(x, y) == false) return;

	m_Board[x][y] = shipSize * 10;
}

void AIBoard::MarkAIBoardWithSunkenShip(ShipType shipType, Coordinate lastAttackCoord)
{
	int shipSize = Ship::GetShipSize(shipType);

	if (CheckIfSunkAndMark(lastAttackCoord.x, lastAttackCoord.y, shipSize, UP)) return;
	if (CheckIfSunkAndMark(lastAttackCoord.x, lastAttackCoord.y, shipSize, DOWN)) return;
	if (CheckIfSunkAndMark(lastAttackCoord.x, lastAttackCoord.y, shipSize, LEFT)) return;
	if (CheckIfSunkAndMark(lastAttackCoord.x, lastAttackCoord.y, shipSize, RIGHT)) return;
}

bool AIBoard::CheckIfSunkAndMark(int x, int y, int shipSize, Direction direction)
{
	bool directionCorrect = false;
	int x_initial = x;
	int y_initial = y;

	for (int i = 0; i < shipSize; i++)
	{
		int x_shift = 0;
		int y_shift = 0;

		switch (direction)
		{
		case UP: x_shift = i * -1;
			break;
		case DOWN: x_shift = i;
			break;
		case LEFT: y_shift = i * -1;
			break;
		case RIGHT: y_shift = i;
			break;
		default:
			break;
		}

		x = x_initial + x_shift;
		y = y_initial + y_shift;

		if (MapBoundaryCheck(x, y) == false)
		{
			directionCorrect = false;
			break;
		}

		if (m_Board[x][y] == 1)
		{
			directionCorrect = true;
		}
		else
		{
			directionCorrect = false;
			break;
		}
	}

	x = x_initial;
	y = y_initial;

	if (directionCorrect)
	{
		for (int i = 0; i < shipSize; i++)
		{
			int x_shift = 0;
			int y_shift = 0;

			switch (direction)
			{
			case UP: x_shift = i * -1;
				break;
			case DOWN: x_shift = i;
				break;
			case LEFT: y_shift = i * -1;
				break;
			case RIGHT: y_shift = i;
				break;
			default:
				break;
			}

			x = x_initial + x_shift;
			y = y_initial + y_shift;

			m_Board[x][y] = shipSize * 10;
		}
	}

	return directionCorrect;
}

/*
if unsunk but hit ship exists, then return true
returns false if there are no hit but unsunk ships
*/
bool AIBoard::CheckForHitButUnsunkShips()
{
	bool unsunkButHitShipExists = false;

	for (int i = 0; i < m_Height; i++)
	{
		for (int j = 0; j < m_Width; j++)
		{
			if (m_Board[i][j] == 1)
			{
				unsunkButHitShipExists = true;
				break;
			}
		}
	}

	return unsunkButHitShipExists;
}
Coordinate AIBoard::GetUnsunkHitCoordinate()
{
	Coordinate unsunkCoordinate;

	for (int i = 0; i < m_Height; i++)
	{
		for (int j = 0; j < m_Width; j++)
		{
			if (m_Board[i][j] == 1)
			{
				unsunkCoordinate.x = i;
				unsunkCoordinate.y = j;
				break;
			}
		}
	}

	return unsunkCoordinate;
}