#include "stdafx.h"
#include "AI.h"
#include "AIBoard.h"
#include "Board.h"
#include "Position.h"
#include "AI_HeatMap.h"
#include "Ship.h"

AI::AI()
{
	// call setup boards and add ships after creating AI class
	m_CurrentAttackMode = TARGETMODE;
	m_ShipsToTargetList.clear();
	m_TargetMode_AttackList.clear();
	m_ContinuousMissCount = 0;
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
	m_AIHeatMap->ShipDestroyed(shipType);
	AIOnDestroy(lastAttackCoord);

	if (m_Enemy_AIBoard->CheckForHitButUnsunkShips() == false)
	{
		m_TargetMode_AttackList.clear();
		m_CurrentAttackMode = HUNTMODE;
	}
}
void AI::ShowAIBoard()
{
	m_Enemy_AIBoard->PrintBoard();
}

Coordinate AI::ComputeNextAttack()
{
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
Coordinate AI::RunHuntMode()
{
	m_AIHeatMap->GenerateHeatMap();

	if (m_ContinuousMissCount > 5
		&& m_ContinuousMissCount % 2 == 0)
	{
		Coordinate coord = m_AIHeatMap->GetColdestAttackCoord();
		return coord;
	}
	else
	{
		Coordinate coord = m_AIHeatMap->GetHottestAttackCoord();
		return coord;
	}
	/*int maxHeight = m_Enemy_AIBoard->GetMaxHeight();
	int maxWidth = m_Enemy_AIBoard->GetMaxWidth();

	do{
		coord.x = rand() % maxWidth;
		coord.y = rand() % maxHeight;

	} while (!m_Enemy_AIBoard->IsNextAttackValid(coord));*/

	
	// generate heat map
	// generate random coordinate to attack 
}
Coordinate AI::RunTargetMode()
{
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
		Coordinate unsunkHitCoordinate = m_Enemy_AIBoard->GetUnsunkHitCoordinate();

		AIPlanNextAttack(GetModifiedCoordinate(unsunkHitCoordinate, UP));
		AIPlanNextAttack(GetModifiedCoordinate(unsunkHitCoordinate, DOWN));
		AIPlanNextAttack(GetModifiedCoordinate(unsunkHitCoordinate, LEFT));
		AIPlanNextAttack(GetModifiedCoordinate(unsunkHitCoordinate, RIGHT));
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