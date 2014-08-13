#pragma once
#include "Board.h"
#include "Enums.h"

class Player;
struct Position;

class GameManager
{
public:
	GameManager();
	~GameManager();

	int WhoWinner();
	int GetNowTurn();
	void TurnChange();
	void StartGame();
private:
	void DeleteShip();
	void PlayGameLoop();
	GameStatus CheckGameStatus();

private:
	Board m_Board_Attacker;
	Board m_Board_Defender;
	Player* m_Attacker;
	Player* m_Defender;
	GameStatus m_Status;
	Turn m_Turn;
};

