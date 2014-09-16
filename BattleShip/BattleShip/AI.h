#pragma once

#include <vector>
#include "Enums.h"
#include "Position.h"

class Ship;
class AIBoard;
class Board;
class AI_HeatMap;



enum ATTACK_MODE
{
	HUNTMODE,
	TARGETMODE,
};

class AI
{
public:
	AI();
	~AI();
	
	void SetUpBoards(Board* myBoard);
	void AddShipToTargetList(Ship* ship);
	void ProcessLastHitResult(HitResult lastHitResult, Coordinate lastAttackCoordinate);
	void ShowAIBoard();
	Coordinate ComputeNextAttack();

	void	AddRandomPlacementData(std::string dataStr);

private:
	Coordinate RunHuntMode();
	Coordinate RunTargetMode();
	void GenerateHeatMap();
	void InitializeBoards();
	void AIOnHitLogic(Coordinate hitCoordinate);
	void AIUpdateBoardWithMiss(Coordinate missedCoordinate);
	void AIOnDestroy(Coordinate hitCoordinate);
	void AIPlanNextAttack(Coordinate potentialCoordinate);
	Coordinate GetModifiedCoordinate(Coordinate coordinate, Direction direction);

	Coordinate GetNextOverlapCoordinate();
	int GetMapOverlapProbability();

private:
	int					m_ContinuousMissCount;
	unsigned int		m_OverlapCandidateIndex;
	AIBoard*			m_Enemy_AIBoard;
	ATTACK_MODE			m_CurrentAttackMode;
	AI_HeatMap*			m_AIHeatMap;
		
	std::vector<Ship*>			m_ShipsToTargetList;
	std::vector<Coordinate>		m_TargetMode_AttackList;
	std::vector<Coordinate>		m_HitSuccessList;
	std::vector<int*>			m_PossibleRandomPlacementList;

	int					GetShipSize(ShipType shipType);
	void				HandleShipDestroyed(ShipType shipType, Coordinate lastAttackCoord);
};

