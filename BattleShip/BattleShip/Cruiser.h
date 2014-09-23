#pragma once
#include "ship.h"
class Cruiser : public Ship
{
public:
	Cruiser(int displayFlag);
	~Cruiser();

	HitResult HitCheck(Position hitPos);
};

