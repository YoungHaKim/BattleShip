#pragma once
#include "Board.h"
#include "Enums.h"

class Ship;

class AI_HeatMap :
	public Board
{
public:
	AI_HeatMap(int width, int height) : Board(width, height) 
	{
		//printf_s("New AI Heat Map Generated! \n");
	}
	~AI_HeatMap();

	void		AddAttack(Coordinate coordinate);
	void		AddHit(Coordinate coordinate);
	void		ShipDestroyed(ShipType shiptype);
	void		AddShip(Ship* ship);
	void		GenerateHeatMap();

	Coordinate	GetHottestAttackCoord();
	Coordinate	GetColdestAttackCoord();

private:
	std::vector<Ship*>	m_ShipPtrList;

	void	ClearHeatValues();
	void	RunHeatMapPass(int shipSize);
	void	MarkRange(int x, int y, int size, Direction direction);
	bool	RangeIsValid(int x, int y, int size, Direction direction);
};

