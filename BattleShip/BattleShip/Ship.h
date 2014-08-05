#pragma once
#include<string>
#include "Enums.h"

struct Position
{
	char m_X;
	char m_Y;
};


class Ship
{
public:
	Ship();
	~Ship();

	void AddPosition(char x, char y);
	void AddPosition(Position pos);
	HitResult HitCheck(Position hitPos);
	bool PositionCheck( int posX, int posY );
	int GetHP();
	int GetMaxHP() { return m_MaxHP; }
	void Print();
	ShipType GetShipType() { return m_Type; }
	
protected:
	std::string m_Name;
	ShipType m_Type;
	Position m_Pos[5];
	int m_HP;
	int m_MaxHP;
};