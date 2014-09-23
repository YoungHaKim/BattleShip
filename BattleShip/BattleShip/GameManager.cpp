#include "stdafx.h"
#include "GameManager.h"
#include "Player.h"
#include "Board.h"
#include "Ship.h" 
#include "Position.h"

#define DATA_GENERATE_MODE	false	// AI 용 맵 데이터를 생성하는 모드인지, true라면 게임 결과조차 화면에 출력되지 않는다


GameManager::GameManager()
{
	m_Status = GAMEOVER;
	m_Turn = ATTACKER;
}
GameManager::~GameManager()
{
}

int GameManager::StartGame(bool autoFlag)
{

	int boardSizeWidth = 8;
	int boardSizeHeight = 8;

	m_Attacker = new Player(boardSizeWidth, boardSizeHeight);
	m_Defender = new Player(boardSizeWidth, boardSizeHeight);

	m_Attacker->SetPlayerType(COMPUTER_AI);

	m_Attacker->SetPlayerName("AI_Advanced_Player");
	m_Defender->SetPlayerName("Random Shooter");

	m_Board_Attacker = m_Attacker->GetMyBoard();
	m_Board_Defender = m_Defender->GetMyBoard();
	m_Attacker->SetEnemyBoard(m_Board_Defender);
	m_Defender->SetEnemyBoard(m_Board_Attacker);

	m_Attacker->SetupShips(false);
	m_Defender->SetupShips(false);

	m_Status = PLAYING;
	m_Turn = ATTACKER;

	int turnCount = PlayGameLoop(autoFlag);


	if (!autoFlag)
	{
		if (m_Attacker->IsAllSunk())
		{
			printf_s("%s Lost! Defender WINS!\n", m_Attacker->GetPlayerName().c_str());
		}
		else
		{
			printf_s("%s Lost! Attacker WINS!\n", m_Defender->GetPlayerName().c_str());
		}
	}

	return turnCount;

	//delete에서 오류가 발생하는데 이유를 모르겠음..
	//delete m_Attacker;
	//delete m_Defender;
}
int GameManager::PlayGameLoop(bool autoFlag)
{
	int turnCount = 0;

	while (m_Status == PLAYING)
	{
		++turnCount;

		if (!autoFlag){
			system("cls");
			printf_s("Turn: %d \n\n", turnCount);
		}
		
		Position attackPosition;

		


		switch (m_Turn)
		{
		case ATTACKER:
		{

			attackPosition = m_Attacker->Attack();
			HitResult hitResult = m_Defender->DoHitCheck(attackPosition);


			if (!autoFlag){
				printf_s("\n%s Turn \n=> %c %c\n", m_Attacker->GetPlayerName().c_str(),
					'A' + attackPosition.m_X, '1' + attackPosition.m_Y);
				PrintHitResult(hitResult);
			}
			

			m_Attacker->ProcessHitResult(hitResult);
			m_Board_Defender->ProcessAttack(attackPosition);



			m_Turn = DEFENDER;
			break;
		}
		case DEFENDER:
		{

			attackPosition = m_Defender->Attack();
			HitResult hitResult = m_Attacker->DoHitCheck(attackPosition);

			if (!autoFlag)
			{
				printf_s("\n%s Turn \n=> %c %c\n", m_Defender->GetPlayerName().c_str(),
					'A' + attackPosition.m_X, '1' + attackPosition.m_Y);
				PrintHitResult(hitResult);
			}

			m_Defender->ProcessHitResult(hitResult);
			m_Board_Attacker->ProcessAttack(attackPosition);

			m_Turn = ATTACKER;
			break;
		}
		default:
			break;
		}

		if (!autoFlag)
		{
			printf_s("\n");
			m_Attacker->GetMyBoard()->PrintBoard();
			m_Attacker->PrintAIBoard();
			m_Defender->GetMyBoard()->PrintBoard();
			
		}

		m_Status = CheckGameStatus();

		if (m_Status == GAMEOVER)
			break;

		
		if (autoFlag == false)
		{
			printf_s("Press any key for next turn\n");
			getchar();	//매 턴마다 멈추기 위해
		}
	}

	if (!DATA_GENERATE_MODE)
	{
		printf_s("Game over! total turns: %d\n", turnCount / 2);
	}

	if (!autoFlag)
		printf_s("Press Enter to continue\n");


	// record ship placement of high count games
	// use against my opponents
	// as monte carlo killer
	if (turnCount / 2 >= 50)
	{
		std::ofstream fileStream;
		fileStream.open("shipPlacement.txt", std::ios::out | std::ios::app);

		fileStream << m_Defender->GetMyBoard()->GetBoardAsString() << std::endl;
		fileStream.close();

		if (!DATA_GENERATE_MODE)
		{
			m_Attacker->GetMyBoard()->PrintBoard();
			m_Defender->GetMyBoard()->PrintBoard();
		}
		fflush(stdin);
		//getchar();
	}

	return turnCount / 2;
}
void GameManager::PrintHitResult(HitResult hitResult)
{

	switch (hitResult)
	{
	case HIT:
		printf_s(" HIT!!");
		break;
	case MISS:
		printf_s(" MISS!!");
		break;
	case DESTROY:
		printf_s(" DESTROY!!");
		break;
	case DESTROY_AIRCRAFT:
		printf_s(" DESTROY_AIRCRAFT!!");
		break;
	case DESTROY_BATTLESHIP:
		printf_s(" DESTROY_BATTLESHIP!!");
		break;
	case DESTROY_CRUISER:
		printf_s(" DESTROY_CRUISER!!");
		break;
	case DESTROY_DESTROYER:
		printf_s(" HITDESTROY_DESTROYER!!");
		break;
	default:
		break;
	}
}
GameStatus GameManager::CheckGameStatus()
{
	if (m_Attacker->IsAllSunk() 
		|| m_Defender->IsAllSunk())
		return GAMEOVER;

	return PLAYING;
}
