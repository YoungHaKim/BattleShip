#pragma once
#include "ship.h"
class BattleShip : public Ship
{
public:
	BattleShip(int displayFlag);
	~BattleShip();


	HitResult HitCheck(Position hitPos);
};

