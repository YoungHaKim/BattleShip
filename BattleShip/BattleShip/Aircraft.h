#pragma once
#include "ship.h"
class AirCraft : public Ship
{
public:
	AirCraft(int displayFlag);
	~AirCraft();

	HitResult HitCheck(Position hitPos);
};

