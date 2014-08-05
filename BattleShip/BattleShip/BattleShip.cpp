#include "stdafx.h"
#include "BattleShip.h"


BattleShip::BattleShip()
{
	m_MaxHP = 4;
	m_HP = m_MaxHP;
	m_Type = BATTLESHIP;
	m_Name = "BattleShip";
}


BattleShip::~BattleShip()
{
}
