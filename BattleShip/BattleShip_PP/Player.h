#pragma once
#include "stdafx.h"
#include "Board.h"
#include "AirCraft.h"
#include "BattleShip.h"
#include "Cruiser.h"
#include "Destroyer.h"
#include "Enums.h"

class Player
{
public:
	Player();
	~Player();

public:
	void Submit();
	void SetupShips(); // random location, random direction, check if within map bounds, ships can't overlap
	void Attack();
	void GiveUp();

protected:
	AirCraft	m_Aircraft;
	BattleShip	m_Battleship;
	Cruiser		m_Cruiser;
	Destroyer	m_Destroyer[2];

protected:
	bool IsValidPosition(int startX, int startY, int maxHp, Direction direction);

private:
	Board myBoard;
	Board enemyBoard;
	//Ship* shipList;
	int type;
};

