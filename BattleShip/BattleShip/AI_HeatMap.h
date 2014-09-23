#pragma once
#include "Board.h"
#include "Enums.h"

class Ship;
class AIBoard;

class AI_HeatMap :
	public Board
{
public:
	AI_HeatMap(int width, int height) : Board(width, height)  {}
	~AI_HeatMap();

	void		AddAttack(Coordinate coordinate);
	void		AddHit(Coordinate coordinate);
	void		ShipDestroyed(ShipType shiptype, Coordinate lastAttackCoord);
	void		AddShip(Ship* ship);
	void		GenerateHeatMap(AIBoard* aiBoard);
	void		GenerateTargetHeatMap(AIBoard* aiBoard);

	Coordinate	GetHottestAttackCoord();
	Coordinate	GetColdestAttackCoord();

private:
	std::vector<Ship*>	m_ShipPtrList;
	AIBoard*			m_AIBoardRef;

private:
	void	ResetHeatMap();
	void	RunHeatMapPass(int shipSize);
	void	RunTargetHeatMapPass(int shipSize);
	void	MarkRange(int x, int y, int size, Direction direction);
	void	MarkRangeTargetMode(int overlapCount, int x, int y, int size, Direction direction);

	bool	RangeIsValid(int x, int y, int size, Direction direction);
	bool	RangeIsValidTargetMode(int x, int y, int size, Direction direction);
	bool	CheckIfSunkAndMark(int x, int y, int shipSize, Direction direction);

	int		RangePassesHit(int x, int y, int size, Direction direction);

};

