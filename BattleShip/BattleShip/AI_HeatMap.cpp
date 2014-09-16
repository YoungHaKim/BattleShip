#include "stdafx.h"
#include "AI_HeatMap.h"
#include "Ship.h"
#include "Enums.h"
#include "AIBoard.h"

AI_HeatMap::~AI_HeatMap()
{
}

void	AI_HeatMap::AddAttack(Coordinate coordinate)
{
	if (MapBoundaryCheck(coordinate) == false) return;

	m_Board[coordinate.x][coordinate.y] = -1;
}
void	AI_HeatMap::AddHit(Coordinate coordinate)
{
	if (MapBoundaryCheck(coordinate) == false) return;

	m_Board[coordinate.x][coordinate.y] = -10;
}
void	AI_HeatMap::ShipDestroyed(ShipType shiptype, Coordinate lastAttackCoord)
{
	int shipSize = 0;

	for (auto it = m_ShipPtrList.begin(); it != m_ShipPtrList.end();)
	{
		Ship* ship = *it;

		if (ship->GetShipType() == shiptype) {
			shipSize = ship->GetMaxHP();
			it = m_ShipPtrList.erase(it);
			break;
		}
		else
		{
			++it;
		}
	}

	if (CheckIfSunkAndMark(lastAttackCoord.x, lastAttackCoord.y, shipSize, UP)) return;
	if (CheckIfSunkAndMark(lastAttackCoord.x, lastAttackCoord.y, shipSize, DOWN)) return;
	if (CheckIfSunkAndMark(lastAttackCoord.x, lastAttackCoord.y, shipSize, LEFT)) return;
	if (CheckIfSunkAndMark(lastAttackCoord.x, lastAttackCoord.y, shipSize, RIGHT)) return;

}
bool AI_HeatMap::CheckIfSunkAndMark(int x, int y, int shipSize, Direction direction)
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

			m_Board[x][y] = -1;
		}
	}

	return directionCorrect;
}

void	AI_HeatMap::AddShip(Ship *ship)
{
	//if (ship->GetShipType() == AIRCRAFT)
		m_ShipPtrList.push_back(ship);
}

void AI_HeatMap::ResetHeatMap()
{
	for (int i = 0; i < m_Height; i++)
	{
		for (int j = 0; j < m_Width; j++)
		{
			int **aiBoard = m_AIBoardRef->GetBoardRawData();
			if (aiBoard[i][j] <= 0)
			{
				m_Board[i][j] = aiBoard[i][j];
			}
			else if (aiBoard[i][j] == 1)
			{
				m_Board[i][j] = -10;
			}
			else if (aiBoard[i][j] >= 10)
			{
				m_Board[i][j] = -1;
			}
			
		}
	}
}
void AI_HeatMap::GenerateHeatMap(AIBoard* aiBoard)
{
	m_AIBoardRef = aiBoard;

	ResetHeatMap();

	for (auto pShip : m_ShipPtrList)
	{
		int hp = pShip->GetMaxHP();

		RunHeatMapPass(hp);
	}

	//printf_s("Generated Heat Map! Ship count: %d \n", m_ShipPtrList.size());
	//PrintBoard();
}
void AI_HeatMap::GenerateTargetHeatMap(AIBoard* aiBoard)
{
	//printf_s("Generating targetHeatMap\n");
	

	m_AIBoardRef = aiBoard;

	ResetHeatMap();
	//PrintBoard();

	for (auto pShip : m_ShipPtrList)
	{
		int hp = pShip->GetMaxHP();

		RunTargetHeatMapPass(hp);
	}

	//printf_s("Generated Heat Map! Ship count: %d \n", m_ShipPtrList.size());
	//PrintBoard();
}

Coordinate	AI_HeatMap::GetHottestAttackCoord()
{
	int max_x = 0;
	int max_y = 0;

	// find coordinates of the top 2 max probability number
	for (int i = 0; i < m_Height; i++)
	{
		for (int j = 0; j < m_Width; j++)
		{
			if (m_Board[i][j] > m_Board[max_x][max_y])
			{
					max_x = i;
					max_y = j;
			}
		}
	}


	// save the coordinates of the squares that equal the max value probability
	int maxValue = m_Board[max_x][max_y];
	std::vector<Coordinate> coordVector;
	for (int i = 0; i < m_Height; i++)
	{
		for (int j = 0; j < m_Width; j++)
		{
			if (m_Board[i][j] == maxValue)
			{
					Coordinate coord;
					coord.x = i;
					coord.y = j;

					coordVector.push_back(coord);
				
			}
		}
	}
	
	// if there is a tie, then run a random function to select coord to attack
	if (coordVector.size() > 1)
	{ 
		int randNum = rand() % coordVector.size();
		return coordVector[randNum];
	}
	else
	{
		return coordVector[0];
	}

}
Coordinate	AI_HeatMap::GetColdestAttackCoord()
{
	int min_x = 0;
	int min_y = 0;

	// find coordinates of the top 2 max probability number
	for (int i = 0; i < m_Height; i++)
	{
		for (int j = 0; j < m_Width; j++)
		{
			if (m_Board[i][j] > m_Board[min_x][min_y])
			{
				min_x = i;
				min_y = j;
			}
		}
	}


	// save the coordinates of the squares that equal the max value probability
	int maxValue = m_Board[min_x][min_y];
	std::vector<Coordinate> coordVector;
	for (int i = 0; i < m_Height; i++)
	{
		for (int j = 0; j < m_Width; j++)
		{
			if (m_Board[i][j] == maxValue)
			{
				Coordinate coord;
				coord.x = i;
				coord.y = j;

				coordVector.push_back(coord);

			}
		}
	}

	// if there is a tie, then run a random function to select coord to attack
	if (coordVector.size() > 1)
	{
		int randNum = rand() % coordVector.size();
		return coordVector[randNum];
	}
	else
	{
		return coordVector[0];
	}

}

void AI_HeatMap::RunHeatMapPass(int shipSize)
{
	// RIGHT direction
	for (int i = 0; i < m_Height; i++)
	{
		for (int j = 0; j <= m_Width - shipSize; j++)
		{
			// if range is valid
			//printf_s("HeatMap generation size %d, %d, %d RIGHT \n", shipSize, i, j);

			bool result = RangeIsValid(i, j, shipSize, RIGHT);
			if (result == true)
				MarkRange(i, j, shipSize, RIGHT);
		}
	}

	// DOWN direction
	for (int i = 0; i <= m_Height - shipSize; i++)
	{
		for (int j = 0; j < m_Width; j++)
		{
			//printf_s("HeatMap generation size %d, %d, %d DOWN \n", shipSize, i, j);

			bool result = RangeIsValid(i, j, shipSize, DOWN);
			if (result == true)
				MarkRange(i, j, shipSize, DOWN);
		}
	}
}
void AI_HeatMap::RunTargetHeatMapPass(int shipSize)
{
	// RIGHT direction
	for (int i = 0; i < m_Height; i++)
	{
		for (int j = 0; j <= m_Width - shipSize; j++)
		{
			// if range is valid
			//printf_s("HeatMap generation size %d, %d, %d RIGHT \n", shipSize, i, j);

			bool result = RangeIsValidTargetMode(i, j, shipSize, RIGHT);
			if (result == true)
			{
				//MarkRange(i, j, shipSize, RIGHT);

				int overlapCount = RangePassesHit(i, j, shipSize, RIGHT);
				if (overlapCount > 0)
				{
					MarkRangeTargetMode(overlapCount, i, j, shipSize, RIGHT);
				}
			}
		}
	}

	// DOWN direction
	for (int i = 0; i <= m_Height - shipSize; i++)
	{
		for (int j = 0; j < m_Width; j++)
		{
			//printf_s("HeatMap generation size %d, %d, %d DOWN \n", shipSize, i, j);

			bool result = RangeIsValidTargetMode(i, j, shipSize, DOWN);
			if (result == true)
			{
				//MarkRange(i, j, shipSize, DOWN);
				
				int overlapCount = RangePassesHit(i, j, shipSize, DOWN);
				if (overlapCount > 0)
				{
					MarkRangeTargetMode(overlapCount, i, j, shipSize, DOWN);
				}
			}
		}
	}
}

int AI_HeatMap::RangePassesHit(int x, int y, int size, Direction direction)
{
	int squaresTouched = 0;

	if (direction == RIGHT)
	{
		for (int i = y; i < y + size; i++)
		{
			if (MapBoundaryCheck(x, i) == false)
			{
				printf_s("Error in marking range! Boundary Check Failed! \n");
				return 0;
			}

			if (m_Board[x][i] == -10) ++squaresTouched;
		}

		return true;
	}
	else if (direction == DOWN)
	{
		for (int i = x; i < x + size; i++)
		{
			if (MapBoundaryCheck(i, y) == false) {
				printf_s("Error in marking range! Boundary Check Failed! \n");
				return 0;
			}

			if (m_Board[i][y] == -10) ++squaresTouched;
		}

		return squaresTouched;
	}
	else return 0;
}
bool AI_HeatMap::RangeIsValid(int x, int y, int size, Direction direction)
{
	if (direction == RIGHT)
	{
		for (int i = y; i < y + size; i++)
		{
			if (MapBoundaryCheck(x, i) == false)
			{
				printf_s("Error in marking range! Boundary Check Failed! \n");
				return false;
			}

			if (m_Board[x][i] == -1) return false;
		}

		return true;
	}
	else if (direction == DOWN)
	{
		for (int i = x; i < x + size; i++)
		{
			if (MapBoundaryCheck(i, y) == false) {
				printf_s("Error in marking range! Boundary Check Failed! \n");
				return false;
			}

			if (m_Board[i][y] == -1) return false;
		}

		return true;
	}
	else return false;
}
bool AI_HeatMap::RangeIsValidTargetMode(int x, int y, int size, Direction direction)
{
	if (direction == RIGHT)
	{
		bool hitExistsInRange = false;
		for (int i = y; i < y + size; i++)
		{
			if (MapBoundaryCheck(x, i) == false)
			{
				printf_s("Error in marking range! Boundary Check Failed! \n");
				return false;
			}

			if (m_Board[x][i] == -1) return false;
			if (m_Board[x][i] == -10)
				hitExistsInRange = true;
		}
		if (hitExistsInRange == false)
			return false;

		return true;
	}
	else if (direction == DOWN)
	{
		bool hitExistsInRange = false;
		for (int i = x; i < x + size; i++)
		{
			if (MapBoundaryCheck(i, y) == false) {
				printf_s("Error in marking range! Boundary Check Failed! \n");
				return false;
			}

			if (m_Board[i][y] == -1) return false;
			if (m_Board[i][y] == -10)
				hitExistsInRange = true;
		}
		if (hitExistsInRange == false)
			return false;

		return true;
	}
	else
		return false;
}
void AI_HeatMap::MarkRange(int x, int y, int size, Direction direction)
{
	if (direction == RIGHT)
	{
		for (int i = y; i < y + size; i++)
		{
			if (MapBoundaryCheck(x, i) == false) printf_s("Error in marking range! Boundary Check Failed! \n");

			if (m_Board[x][i] != -1
				&& m_Board[x][i] != -10)
			{
				m_Board[x][i] += 1;
			}
			else
			{
				//printf_s("Error in marking range! Trying to mark -1! \n");
			}
		}

	}
	else if (direction == DOWN)
	{
		for (int i = x; i < x + size; i++)
		{
			if (MapBoundaryCheck(i, y) == false) printf_s("Error in marking range! Boundary Check Failed! \n");
			if (m_Board[i][y] != -1
				&& m_Board[i][y] != -10)
			{
				m_Board[i][y] += 1;
			}
			else
			{
				//printf_s("Error in marking range! Trying to mark -1! \n");
			}
		}

	}

}
void AI_HeatMap::MarkRangeTargetMode(int squaresTouched, int x, int y, int size, Direction direction)
{
	if (direction == RIGHT)
	{
		for (int i = y; i < y + size; i++)
		{
			if (MapBoundaryCheck(x, i) == false) printf_s("Error in marking range! Boundary Check Failed! \n");

			if (m_Board[x][i] != -1
				&& m_Board[x][i] != -10)
			{
				m_Board[x][i] += squaresTouched;
			}
			else
			{
				//printf_s("Error in marking range! Trying to mark -1! \n");
			}
		}

	}
	else if (direction == DOWN)
	{
		for (int i = x; i < x + size; i++)
		{
			if (MapBoundaryCheck(i, y) == false) printf_s("Error in marking range! Boundary Check Failed! \n");
			if (m_Board[i][y] != -1
				&& m_Board[i][y] != -10)
			{
				m_Board[i][y] += squaresTouched;
			}
			else
			{
				//printf_s("Error in marking range! Trying to mark -1! \n");
			}
		}

	}

}