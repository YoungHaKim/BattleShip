#include "stdafx.h"
#include "Destroyer.h"


Destroyer::Destroyer()
{
	m_MaxHP = 2;
	m_HP = m_MaxHP;
	m_Type = DESTROYER;
	m_Name = "Destroyer";
}


Destroyer::~Destroyer()
{
}
