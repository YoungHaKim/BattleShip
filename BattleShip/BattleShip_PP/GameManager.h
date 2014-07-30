#pragma once
#include "Board.h"

class GameManager
{
public:
	GameManager();
	~GameManager();
private:
	Board b1;
	Board b2;
	int nowTurn;
	void DeleteShip();

public:
	int whoWinner();
	int GetNowTurn();
	void turnChange();

};

