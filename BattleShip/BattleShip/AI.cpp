#include "stdafx.h"
#include "AI.h"
#include "AIBoard.h"
#include "Board.h"
#include "Position.h"
#include "AI_HeatMap.h"
#include "Ship.h"

AI::AI()
{
	m_TotalTurnCount = 0;
	// call setup boards and add ships after creating AI class
	m_CurrentAttackMode = TARGETMODE;
	m_ShipsToTargetList.clear();
	m_TargetMode_AttackList.clear();
	m_ContinuousMissCount = 0;
	m_AttackLogic = STANDARD;
}
AI::~AI()
{
	delete(m_Enemy_AIBoard);
	delete(m_AIHeatMap);
}

void AI::SetUpBoards(Board* myBoard) //, Board* enemyBoard)
{
	if (myBoard == nullptr)
	{
		printf_s("Error in setting up AI logic! Board is null!\n");
		return;
	}

	int width = myBoard->GetMaxWidth();
	int height = myBoard->GetMaxHeight();

	m_Enemy_AIBoard = new AIBoard(width, height);
	m_Enemy_AIBoard->SetBoardName("AIBOARD");
	m_AIHeatMap = new AI_HeatMap(width, height);
	m_AIHeatMap->SetBoardName("AI HeatMAP");
}
void AI::AddShipToTargetList(Ship* ship)
{
	m_ShipsToTargetList.push_back(ship);
	m_AIHeatMap->AddShip(ship);
}
void AI::SetAILogic(AttackLogic attackLogic)
{
	m_AttackLogic = attackLogic;
}
void AI::ProcessLastHitResult(HitResult lastHitResult, Coordinate lastAttackCoordinate)
{
	switch (lastHitResult)
	{
	case HIT:
		m_ContinuousMissCount = 0;
		AIOnHitLogic(lastAttackCoordinate);		
		break;

	case MISS:
		++m_ContinuousMissCount;
		AIUpdateBoardWithMiss(lastAttackCoordinate);
		break;

	case DESTROY:
		AIOnDestroy(lastAttackCoordinate);
		break;

	case DESTROY_AIRCRAFT:
		// examine last coordinates, and look for contiguous mark as destroyed
		HandleShipDestroyed(AIRCRAFT, lastAttackCoordinate);		
		break;

	case DESTROY_BATTLESHIP:
		HandleShipDestroyed(BATTLESHIP, lastAttackCoordinate);
		break;

	case DESTROY_CRUISER:
		HandleShipDestroyed(CRUISER, lastAttackCoordinate);
		break;

	case DESTROY_DESTROYER:
		HandleShipDestroyed(DESTROYER, lastAttackCoordinate);
		break;

	default:
		break;
	}
}
void AI::HandleShipDestroyed(ShipType shipType, Coordinate lastAttackCoord)
{
	m_Enemy_AIBoard->MarkAsHit(lastAttackCoord);
	m_Enemy_AIBoard->MarkAIBoardWithSunkenShip(shipType, lastAttackCoord);
	m_AIHeatMap->ShipDestroyed(shipType, lastAttackCoord);
	AIOnDestroy(lastAttackCoord);

	m_TargetMode_AttackList.clear();

	if (m_Enemy_AIBoard->CheckForHitButUnsunkShips() == false)
	{
		//m_TargetMode_AttackList.clear();
		m_CurrentAttackMode = HUNTMODE;
	}
}
void AI::ShowAIBoard()
{
	m_Enemy_AIBoard->PrintBoard();
	m_AIHeatMap->PrintBoard();
}


// returns the number of positions that overlap
// so the max is 5 + 4 +3 + 2*2 = 16
// Loop through the vector of samples
// see how many positions overlap
int AI::GetMapOverlapProbability()
{
	if (m_PossibleRandomPlacementList.empty()) return 0;


	// convert enemy map into int array
	int* enemyMapArr = m_Enemy_AIBoard->GetBoardAsIntArray();

	int hitCount = 0;
	for (int i = 0; i < BOARD_SIZE; i++)
	{
		if (enemyMapArr[i] > 0)
			++hitCount;
	}
	if (hitCount < 1) return 0;

	int indexOfMaxOverlapArr = 0;
	int maxOverlapCount = 0;

	for (unsigned int i = 0; i < m_PossibleRandomPlacementList.size(); ++i)
	{
		int hitOverlapCount = 0;
		int* sampleArr = m_PossibleRandomPlacementList[i];

		for (int j = 0; j < BOARD_SIZE; j++)
		{
			//printf_s("sampleArr[%d]=%d, enemyMap[%d]=%d \n", j, sampleArr[j], j, enemyMapArr[j]);

			if (sampleArr[j] == enemyMapArr[j]
				&& sampleArr[j] > 0)				// count the number of overlapped non zeros
			{
				++hitOverlapCount;
			}
			else if (sampleArr[j] > 0 && enemyMapArr[j] < 0) // ship exists in sample but we missed in reality
			{
				continue;
			}
		}

		if (hitOverlapCount > maxOverlapCount)
		{
			maxOverlapCount = hitOverlapCount;
			indexOfMaxOverlapArr = i;
		}
	}

	m_OverlapCandidateIndex = indexOfMaxOverlapArr;

	return maxOverlapCount;
}
Coordinate AI::ComputeNextAttack()
{
	++m_TotalTurnCount;

	Coordinate nextAttackCoord;

	switch (m_CurrentAttackMode)
	{
	case HUNTMODE:
		nextAttackCoord = RunHuntMode();
		break;

	case TARGETMODE:
		nextAttackCoord = RunTargetMode();
		break;

	default:
		printf_s("Error in AI! Unsupported Mode!\n");
	
		nextAttackCoord.x = 0;
		nextAttackCoord.y = 0;
		break;
	}	

	m_AIHeatMap->AddAttack(nextAttackCoord);
	//m_AIHeatMap->GenerateHeatMap();

	return nextAttackCoord;
}
// assuming we ran AI::GetMapOverlapProbability()
// we will get the next hit candidate
Coordinate AI::GetNextOverlapCoordinate()
{
	if (m_OverlapCandidateIndex < 0 || 
		(unsigned int)m_OverlapCandidateIndex > m_PossibleRandomPlacementList.size())
	{
		printf_s("Error in Coordinate AI::GetNextOverlapCoordinate(), \
				 m_OverlapCandidateIndex < 0 || m_OverlapCandidateIndex > m_PossibleRandomPlacementList.size() \n");
		Coordinate coord;
		coord.x = 1;
		coord.y = 1;
		return coord;
	}

	int* enemyMapArr = m_Enemy_AIBoard->GetBoardAsIntArray();
	int* sampleArr = m_PossibleRandomPlacementList[m_OverlapCandidateIndex];

	for (int j = 0; j < BOARD_SIZE; j++)
	{
		if (sampleArr[j] > 0 && enemyMapArr[j] == 0)  // ship exists in sample but we haven't targeted yet!
		{
			int colIndex = j % BOARD_WIDTH;
			int rowIndex = (int)(j / BOARD_WIDTH);

			Coordinate coord;
			coord.x = rowIndex;
			coord.y = colIndex;
			return coord;
		}
	}

	Coordinate coord;
	coord.x = 1;
	coord.y = 1;
	return coord;
}
Coordinate AI::GetModifiedCoordinate(Coordinate coordinate, Direction direction)
{
	switch (direction)
	{
	case UP:
		coordinate.y--;
		break;
	case DOWN:
		coordinate.y++;
		break;
	case LEFT:
		coordinate.x--;
		break;
	case RIGHT:
		coordinate.x++;
		break;
	default:
		break;
	}

	return coordinate;
}
Coordinate AI::RunHuntMode()
{
	int overlapProbability = 0; // GetMapOverlapProbability(); // THIS IS SET TO NOT USE OVERLAP FUNCTION
	

	if (overlapProbability > 0
		&& m_OverlapCandidateIndex >= 0
		&& m_OverlapCandidateIndex < m_PossibleRandomPlacementList.size()) // over 50% certainty of overlap
	{
		printf_s("AI is Utilizing random batch data!\n");
		Coordinate coord = GetNextOverlapCoordinate();
		return coord;

	}
	else
	{
		m_AIHeatMap->GenerateHeatMap(m_Enemy_AIBoard);

		if ((m_ContinuousMissCount > 3
			&& m_ContinuousMissCount % 2 == 0)
			|| (m_AttackLogic == CORNERFIRST && m_TotalTurnCount < 5))
		{
			Coordinate coord = m_AIHeatMap->GetColdestAttackCoord();
			return coord;
		}
		else
		{
			Coordinate coord = m_AIHeatMap->GetHottestAttackCoord();
			return coord;
		}
	}
}
Coordinate AI::RunTargetMode()
{
	if (m_Enemy_AIBoard->CheckForHitButUnsunkShips() == false)
	{
		m_CurrentAttackMode = HUNTMODE;
		return RunHuntMode();
	}

	m_AIHeatMap->GenerateTargetHeatMap(m_Enemy_AIBoard);
	Coordinate coord = m_AIHeatMap->GetHottestAttackCoord();
	return coord;


	/****************/
	/*
	if (m_TargetMode_AttackList.empty() == true
		&& m_Enemy_AIBoard->CheckForHitButUnsunkShips() == false)
	{
		//printf_s("AI is back to HUNT MODE!\n");
		m_CurrentAttackMode = HUNTMODE;
		return RunHuntMode();
	}
	else if (m_TargetMode_AttackList.empty() == true
		&& m_Enemy_AIBoard->CheckForHitButUnsunkShips() == true)
	{
		int unsunkCount = 0;
		Coordinate* unsunkArr = m_Enemy_AIBoard->GetUnsunkHitCoordinates(&unsunkCount);

		// if more than 1 unsunk coordinates exist, determine direction
		if (unsunkCount == 1)
		{
			AIPlanNextAttack(GetModifiedCoordinate(unsunkArr[0], UP));
			AIPlanNextAttack(GetModifiedCoordinate(unsunkArr[0], DOWN));
			AIPlanNextAttack(GetModifiedCoordinate(unsunkArr[0], LEFT));
			AIPlanNextAttack(GetModifiedCoordinate(unsunkArr[0], RIGHT));
		}
		else
		{
			for (int i = 0; i < unsunkCount; i++)
			{
				AIPlanNextAttack(GetModifiedCoordinate(unsunkArr[i], UP));
				AIPlanNextAttack(GetModifiedCoordinate(unsunkArr[i], DOWN));
				AIPlanNextAttack(GetModifiedCoordinate(unsunkArr[i], LEFT));
				AIPlanNextAttack(GetModifiedCoordinate(unsunkArr[i], RIGHT));
				//int nextIndex = i + 1;
				//if (nextIndex >= unsunkCount) break; // break if this is the last coordinate

				//if (abs(unsunkArr[i].x - unsunkArr[nextIndex].x) == 1) // vertical
				//{
				//	AIPlanNextAttack(GetModifiedCoordinate(unsunkArr[i], UP));
				//	AIPlanNextAttack(GetModifiedCoordinate(unsunkArr[i], DOWN));
				//	AIPlanNextAttack(GetModifiedCoordinate(unsunkArr[nextIndex], UP));
				//	AIPlanNextAttack(GetModifiedCoordinate(unsunkArr[nextIndex], DOWN));
				//}

				//if (abs(unsunkArr[i].y - unsunkArr[nextIndex].y) == 1) // horizontal
				//{
				//	AIPlanNextAttack(GetModifiedCoordinate(unsunkArr[i], LEFT));
				//	AIPlanNextAttack(GetModifiedCoordinate(unsunkArr[i], RIGHT));
				//	AIPlanNextAttack(GetModifiedCoordinate(unsunkArr[nextIndex], LEFT));
				//	AIPlanNextAttack(GetModifiedCoordinate(unsunkArr[nextIndex], RIGHT));
				//}
			}
		}
	}

	Coordinate coord;


	// Exhaust attack list
	do
	{
		if (m_TargetMode_AttackList.empty())
			return RunHuntMode();

		coord = m_TargetMode_AttackList.back();
		m_TargetMode_AttackList.pop_back();


	} while (m_Enemy_AIBoard->IsNextAttackValid(coord) == false);	// if empty
	
	return coord;
	*/
}
void AI::GenerateHeatMap()
{

}
void AI::InitializeBoards()
{

}
void AI::AIOnHitLogic(Coordinate hitCoordinate)
{
	//printf_s("Hit success! AI is in TARGET MODE!\n");

	m_CurrentAttackMode = TARGETMODE;
	m_Enemy_AIBoard->MarkAsHit(hitCoordinate);
	m_AIHeatMap->AddHit(hitCoordinate);

	m_HitSuccessList.push_back(hitCoordinate);

	if (m_HitSuccessList.empty() == true
		|| m_HitSuccessList.size() == 1)
	{
		AIPlanNextAttack(GetModifiedCoordinate(hitCoordinate, UP));
		AIPlanNextAttack(GetModifiedCoordinate(hitCoordinate, DOWN));
		AIPlanNextAttack(GetModifiedCoordinate(hitCoordinate, LEFT));
		AIPlanNextAttack(GetModifiedCoordinate(hitCoordinate, RIGHT));
	}
	else  // examine if last two hits are contiguous 
	{
		// if last two hits are contigous then attack in the given direction
		Coordinate lastCoordinate = m_HitSuccessList[m_HitSuccessList.size() - 1];
		Coordinate lastlastCoordinate = m_HitSuccessList[m_HitSuccessList.size() - 2];

		if (lastCoordinate.x == lastlastCoordinate.x
			&& abs(lastCoordinate.y - lastlastCoordinate.y) == 1) // two shots hit 
		{
			AIPlanNextAttack(GetModifiedCoordinate(hitCoordinate, UP));
			AIPlanNextAttack(GetModifiedCoordinate(hitCoordinate, DOWN));
		}
		else if (lastCoordinate.y == lastlastCoordinate.y		// ship placement is horizontal
			&& abs(lastCoordinate.x - lastlastCoordinate.x) == 1)
		{
			AIPlanNextAttack(GetModifiedCoordinate(hitCoordinate, LEFT));
			AIPlanNextAttack(GetModifiedCoordinate(hitCoordinate, RIGHT));
		}
		else
		{
			AIPlanNextAttack(GetModifiedCoordinate(hitCoordinate, UP));
			AIPlanNextAttack(GetModifiedCoordinate(hitCoordinate, DOWN));
			AIPlanNextAttack(GetModifiedCoordinate(hitCoordinate, LEFT));
			AIPlanNextAttack(GetModifiedCoordinate(hitCoordinate, RIGHT));
		}
	}

}
void AI::AIPlanNextAttack(Coordinate potentialCoordinate)
{
	if (m_Enemy_AIBoard->IsNextAttackValid(potentialCoordinate) == true)
	{
		m_TargetMode_AttackList.push_back(potentialCoordinate);
	}
	// compute orientation of ship based on last hit data up, left, right, down
	// add surrounding 2-4 directions to target hit list if not already attempted
	// attack!!!
}
void AI::AIUpdateBoardWithMiss(Coordinate missedCoordinate)
{
	// pop off potential targets from list
	// market on aiboard	
	m_Enemy_AIBoard->MarkAsMiss(missedCoordinate);
}
void AI::AIOnDestroy(Coordinate hitCoordinate)
{


	AIOnHitLogic(hitCoordinate);

	//m_CurrentAttackMode = HUNTMODE;
}
void AI::AddRandomPlacementData(std::string dataStr)
{
	int* positionArr = (int*)malloc(sizeof(int) * BOARD_SIZE);

	for (int i = 0; i < BOARD_HEIGHT * BOARD_WIDTH; i++)
	{
		int mapValue = (int)(dataStr.c_str()[i] - '0');
		positionArr[i] = mapValue;
	}

	m_PossibleRandomPlacementList.push_back(positionArr);
}
