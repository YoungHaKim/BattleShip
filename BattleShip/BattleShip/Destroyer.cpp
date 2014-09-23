#include "stdafx.h"
#include "Destroyer.h"


Destroyer::Destroyer(int displayFlag) : Ship(displayFlag)
{
	m_MaxHP = 2;
	m_HP = m_MaxHP;
	m_Type = DESTROYER;
	m_Name = "Destroyer";
}
Destroyer::~Destroyer()
{
}

HitResult Destroyer::HitCheck(Position hitPos)
{
	HitResult hitResult = Ship::HitCheck(hitPos);

	return (hitResult == DESTROY) ? DESTROY_DESTROYER : hitResult;
}
