#pragma once
#include "Enums.h"

class Player;
class Board;
struct Position;

class GameManager
{
public:
	GameManager();
	~GameManager();

	int			StartGame(bool autoFlag);

private:
	int			PlayGameLoop(bool autoFlag);
	void		PrintHitResult(HitResult hitResult);
	GameStatus	CheckGameStatus();

private:
	Board*		m_Board_Attacker;
	Board*		m_Board_Defender;
	Player*		m_Attacker;
	Player*		m_Defender;
	GameStatus	m_Status;
	Turn		m_Turn;
};

