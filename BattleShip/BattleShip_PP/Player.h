#pragma once
#include "stdafx.h"
#include "Board.h"
#include "AirCraft.h"
#include "BattleShip.h"
#include "Cruiser.h"
#include "Destroyer.h"
#include "Enums.h"
#include "Position.h"

class Player
{
public:
	Player();
	~Player();

public:
	//void Submit();
	void SetupShips(); // random location, random direction, check if within map bounds, ships can't overlap
	Position Attack();
	//void GiveUp();
	void PrintShips();
	void ProcessHitResult(HitResult hit);
	HitResult DoHitCheck(Position pos);

protected:
	AirCraft	m_Aircraft;
	BattleShip	m_Battleship;
	Cruiser		m_Cruiser;
	Destroyer	m_Destroyer[2];


protected:
	bool IsValidShipPosition(int startX, int startY, int maxHp, Direction direction);
	void PlaceShip( Ship* ship , int startX , int startY , Direction direction );
	
private:
	Board m_MyBoard;
	Board m_EnemyBoard;
	Ship* m_ShipList[5];
	int m_Type;
};

