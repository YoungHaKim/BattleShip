#pragma once
#include "ship.h"
class Destroyer : public Ship
{
public:
	Destroyer(int displayFlag);
	~Destroyer();


	HitResult HitCheck(Position hitPos);
};

