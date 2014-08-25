#include "stdafx.h"
#include "AI.h"
#include "AIBoard.h"
#include "Board.h"
#include "Position.h"

AI::AI()
{
	// call setup boards and add ships after creating AI class
	m_CurrentAttackMode = TARGETMODE;
	m_ShipsToTargetList.clear();
	m_TargetMode_AttackList.clear();
}


AI::~AI()
{
	delete(m_Enemy_AIBoard);
}

void AI::SetUpBoards(Board* myBoard, Board* enemyBoard)
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
}
void AI::AddShipToTargetList(Ship* ship)
{
	m_ShipsToTargetList.push_back(ship);
}

void AI::ProcessLastHitResult(HitResult lastHitResult, Coordinate lastAttackCoordinate)
{
	switch (lastHitResult)
	{
	case HIT:
		AIOnHitLogic(lastAttackCoordinate);		
		break;

	case MISS:
		AIUpdateBoardWithMiss(lastAttackCoordinate);
		break;

	case DESTROY:
		AIOnDestroy(lastAttackCoordinate);
		break;

	case DESTROY_AIRCRAFT:
		AIOnDestroy(lastAttackCoordinate);
		break;

	case DESTROY_BATTLESHIP:
		AIOnDestroy(lastAttackCoordinate);
		break;

	case DESTROY_CRUISER:
		AIOnDestroy(lastAttackCoordinate);
		break;

	case DESTROY_DESTROYER:
		AIOnDestroy(lastAttackCoordinate);
		break;

	default:
		break;
	}
}
void AI::ShowAIBoard()
{
	m_Enemy_AIBoard->PrintBoard();
}

Coordinate AI::ComputeNextAttack()
{
	switch (m_CurrentAttackMode)
	{
	case HUNTMODE:
		return RunHuntMode();		

	case TARGETMODE:
		return RunTargetMode();		

	default:
		printf_s("Error in AI! Unsupported Mode!\n");
		
		Coordinate coordinate;
		coordinate.x = 0;
		coordinate.y = 0;
		return coordinate;
	}	
}

Coordinate AI::RunHuntMode()
{
	Coordinate coord;
	int maxHeight = m_Enemy_AIBoard->GetMaxHeight();
	int maxWidth = m_Enemy_AIBoard->GetMaxWidth();

	do{
		coord.x = rand() % maxWidth;
		coord.y = rand() % maxHeight;

	} while (!m_Enemy_AIBoard->IsNextAttackValid(coord));

	return coord;
	// generate heat map
	// generate random coordinate to attack 
}
Coordinate AI::RunTargetMode()
{
	if (m_TargetMode_AttackList.empty() == true)
	{
		printf_s("AI is back to HUNT MODE!\n");
		m_CurrentAttackMode = HUNTMODE;
		return RunHuntMode();
	}

	Coordinate coord;

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
	printf_s("Hit success! AI is in TARGET MODE!\n");

	m_CurrentAttackMode = TARGETMODE;
	m_Enemy_AIBoard->MarkAsHit(hitCoordinate);

	AIPlanNextAttack(GetModifiedCoordinate(hitCoordinate, UP));
	AIPlanNextAttack(GetModifiedCoordinate(hitCoordinate, DOWN));
	AIPlanNextAttack(GetModifiedCoordinate(hitCoordinate, LEFT));
	AIPlanNextAttack(GetModifiedCoordinate(hitCoordinate, RIGHT));
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