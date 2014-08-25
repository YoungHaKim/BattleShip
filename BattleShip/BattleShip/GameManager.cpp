#include "stdafx.h"
#include "GameManager.h"
#include "Player.h"
#include "Board.h"
#include "Ship.h" 
#include "Position.h"

GameManager::GameManager()
{
	m_Status = GAMEOVER;
	m_Turn = ATTACKER;
}


GameManager::~GameManager()
{
}

void GameManager::StartGame()
{
	char answer = 'y';

	do
	{

		int boardSizeWidth = 8;
		int boardSizeHeight = 8;

		m_Attacker = new Player(boardSizeWidth, boardSizeHeight);
		m_Defender = new Player(boardSizeWidth, boardSizeHeight);

		m_Attacker->SetPlayerType(COMPUTER_AI);

		m_Attacker->SetPlayerName("Attacker");
		m_Defender->SetPlayerName("Defender");

		m_Board_Attacker = m_Attacker->GetMyBoard();
		m_Board_Defender = m_Defender->GetMyBoard();
		m_Attacker->SetEnemyBoard(m_Board_Defender);
		m_Defender->SetEnemyBoard(m_Board_Attacker);

		m_Attacker->SetupShips();
		m_Defender->SetupShips();

		m_Status = PLAYING;
		m_Turn = ATTACKER;

		PlayGameLoop();

		if (m_Attacker->IsAllSunk())
		{
			printf_s("Attacker Lost! Defender WINS!\n");
		}
		else
		{
			printf_s("Defender Lost! Attacker WINS!\n");
		}

		m_Attacker->GetMyBoard()->PrintBoard();
		m_Attacker->GetEnemyBoard()->PrintBoard();

		printf_s("Play again? 'y', 'n'\n");
		answer = (char)getchar();
		
	} while (answer == 'y' || answer == 'Y');

	//delete에서 오류가 발생하는데 이유를 모르겠음..
	//delete m_Attacker;
	//delete m_Defender;
}

void GameManager::PlayGameLoop()
{
	while (m_Status == PLAYING)
	{
		system("cls");
		Position attackPosition;

		switch (m_Turn)
		{
		case ATTACKER:
		{
			m_Attacker->GetMyBoard()->PrintBoard();
			m_Attacker->GetEnemyBoard()->PrintBoard();

			attackPosition = m_Attacker->Attack();
			HitResult hitResult = m_Defender->DoHitCheck(attackPosition);

			m_Attacker->ProcessHitResult(hitResult);
			m_Board_Defender->ProcessAttack(attackPosition);
			m_Turn = DEFENDER;
			break;
		}
		case DEFENDER:
		{
			m_Attacker->GetMyBoard()->PrintBoard();
			m_Attacker->GetEnemyBoard()->PrintBoard();

			attackPosition = m_Defender->Attack();
			HitResult hitResult = m_Attacker->DoHitCheck(attackPosition);

			m_Defender->ProcessHitResult(hitResult);
			m_Board_Attacker->ProcessAttack(attackPosition);
			m_Turn = ATTACKER;
			break;
		}
		default:
			break;
		}


		printf_s("%s => %c %c\n", m_Turn == ATTACKER ? "Attacker" : "Defender",
			'A' + attackPosition.m_X, '1' + attackPosition.m_Y);

		m_Status = CheckGameStatus();

		printf_s("Press any key for next turn\n");
		getchar();	//매 턴마다 멈추기 위해
	}
}

GameStatus GameManager::CheckGameStatus()
{
	if (m_Attacker->IsAllSunk() 
		|| m_Defender->IsAllSunk())
		return GAMEOVER;

	return PLAYING;
}
