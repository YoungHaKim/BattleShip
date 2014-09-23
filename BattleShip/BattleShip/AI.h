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
	
	void		SetUpBoards(Board* myBoard);
	void		AddShipToTargetList(Ship* ship);
	void		ProcessLastHitResult(HitResult lastHitResult, Coordinate lastAttackCoordinate);
	void		ShowAIBoard();
	void		SetAILogic(AttackLogic attackLogic);
	void		AddRandomPlacementData(std::string dataStr);

	Coordinate	ComputeNextAttack();

private:
	
	void		GenerateHeatMap();
	void		InitializeBoards();
	void		AIOnHitLogic(Coordinate hitCoordinate);
	void		AIUpdateBoardWithMiss(Coordinate missedCoordinate);
	void		AIOnDestroy(Coordinate hitCoordinate);
	void		AIPlanNextAttack(Coordinate potentialCoordinate);
	void		HandleShipDestroyed(ShipType shipType, Coordinate lastAttackCoord);

	int			GetMapOverlapProbability();
	int			GetShipSize(ShipType shipType);

	Coordinate	GetModifiedCoordinate(Coordinate coordinate, Direction direction);
	Coordinate	RunHuntMode();
	Coordinate	RunTargetMode();
	Coordinate	GetNextOverlapCoordinate();

private:
	int							m_TotalTurnCount;
	int							m_ContinuousMissCount;
	unsigned int				m_OverlapCandidateIndex;

	AIBoard*					m_Enemy_AIBoard;
	ATTACK_MODE					m_CurrentAttackMode;
	AI_HeatMap*					m_AIHeatMap;
	AttackLogic					m_AttackLogic;
		
	std::vector<Ship*>			m_ShipsToTargetList;
	std::vector<Coordinate>		m_TargetMode_AttackList;
	std::vector<Coordinate>		m_HitSuccessList;
	std::vector<int*>			m_PossibleRandomPlacementList;

};

