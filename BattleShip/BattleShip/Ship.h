#pragma once
#include "Enums.h"
#include "Position.h"

class Ship
{
public:
	Ship(int displayFlag);
	~Ship();

	void		AddPosition(char x, char y);
	void		AddPosition(Position pos);
	void		Print();

	bool		PositionCheck(int posX, int posY);

	int			GetHP() { return m_HP; }
	int			GetMaxHP() { return m_MaxHP; }
	int			GetDisplayFlag() { return m_ShipDisplayFlag; }

	Position*	GetPositionArr();
	ShipType	GetShipType() { return m_Type; }

	virtual HitResult	HitCheck(Position hitPos);
	static int			GetShipSize(ShipType shipType);
	
protected:

	int			m_HP;
	int			m_MaxHP;
	int			m_ShipDisplayFlag;

	ShipType	m_Type;
	Position	m_Pos[5];
	
	std::string m_Name;

};