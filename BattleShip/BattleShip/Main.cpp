// Main.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//


#include "stdafx.h"

#ifdef _DEBUG

// Debug로 빌드하는 경우
#pragma comment(lib, "BGNL_debug.lib")

#else

// Release로 빌드하는 경우
#pragma comment(lib, "BGNL.lib")

#endif


#include <cmath>

#include "GameManager.h"
#include "Network.h"
#include "PacketType.h"
#include "ShipData.h"

#include "Player.h"

#define PI 3.14
#define SHOW_NETWORK_MSG	false

// Server IP & Port
const char*				IP = "10.73.42.117"; // "10.73.42.117";	// 자기 컴퓨터에서 돌릴 때는 127.0.0.1
const unsigned short	PORT = 9001;
// Attack result string
const char* const ATTACK_RESULT_STR[] = {
	"No Result",
	"Miss",
	"Hit",
	"Aircraft Destroyed",
	"Battleship Destroyed",
	"Cruiser Destroyed",
	"Destroyer Destroyed",
};
// Game Data
char gEnemyMap[MAP_WIDTH][MAP_HEIGHT];

void Initialize()
{
	ZeroMemory(gEnemyMap, sizeof(gEnemyMap));
}
void MakeMapData(char* const mapData)
{
	ZeroMemory(mapData, MAP_SIZE);

	int size, sx, sy, dir, listIdx = 0;
	bool placeable;

	// 배 랜덤배치로 지도를 직접 만드는 방식
	for (int type = MD_NONE + 1; type < MD_END; ++type)
	{
		while (true)
		{
			size = ShipData::SHIP_LEN[type];
			placeable = true;
			dir = rand() % 2;
			if (dir == 0) // hori
			{
				sx = rand() % (MAP_WIDTH - size);
				sy = rand() % MAP_HEIGHT;
			}
			else // vert
			{
				sx = rand() % MAP_WIDTH;
				sy = rand() % (MAP_HEIGHT - size);
			}

			for (int i = 0; i < size && placeable; ++i)
			{
				if (dir == 0 && mapData[sx + i + sy * MAP_WIDTH])
					placeable = false;
				else if (dir == 1 && mapData[sx + (sy + i) * MAP_WIDTH])
					placeable = false;
			}

			if (placeable)
				break;
		}

		for (int i = 0; i < size && placeable; ++i)
		{
			int x, y;
			if (dir == 0) { x = sx + i; y = sy; }
			else  { x = sx; y = sy + i; }
			mapData[x + y * MAP_WIDTH] = type;
		}
	}
}
Coord MakeAttackPos()
{
	Coord pos;
	do
	{
		pos.mX = rand() % MAP_WIDTH;
		pos.mY = rand() % MAP_HEIGHT;
	} while (gEnemyMap[pos.mX][pos.mY]);

	return pos;
}


void HandleMyAttackResult(Player* currentPlayer, const int attackResult, const int x, const int y)
{
	if (currentPlayer == nullptr)
	{
		printf_s("Yo! Major Error in main.HandleMyAttackResult Player ptr is null!\n");
		return;
	}

	if (SHOW_NETWORK_MSG)
		printf_s("Attack result is %s \n", ATTACK_RESULT_STR[attackResult]);

	switch (attackResult)
	{
	case 0:
		break;
	case 1:
		currentPlayer->ProcessHitResult(MISS);
		break;
	case 2:
		currentPlayer->ProcessHitResult(HIT);
		break;
	case 3:
		currentPlayer->ProcessHitResult(DESTROY_AIRCRAFT);
		break;
	case 4:
		currentPlayer->ProcessHitResult(DESTROY_BATTLESHIP);
		break;
	case 5:
		currentPlayer->ProcessHitResult(DESTROY_CRUISER);
		break;
	case 6:
		currentPlayer->ProcessHitResult(DESTROY_DESTROYER);
		break;
	default:
		break;
	}
}

void HandleOpositionAttackResult(Player* currentPlayer, const int attackResult, const int x, const int y)
{
	if (currentPlayer == nullptr)
	{
		printf_s("Yo! Major Error in main.HandleOpositionAttackResult Player ptr is null!\n");
		return;
	}
}

void RunOfflineGame()
{

	GameManager gameManager;


	char answer = 'y';

	do
	{

		fflush(stdin);
		printf_s("Play Auto mode? 'y','n'\n");
		char autoFlagChar = (char)getchar();

		if (autoFlagChar == 'y')
		{
			printf_s("How many repetitions?\n");
			int gameReps = 1;
			scanf_s("%d", &gameReps);
			if (gameReps < 1)
				gameReps = 1;

			int* gameStatArr;
			gameStatArr = (int*)malloc(sizeof(int) * gameReps);

			for (int i = 0; i < gameReps; ++i)
			{
				gameStatArr[i] = gameManager.StartGame(true);
			}

			int total = 0;
			int min = gameStatArr[0];
			int max = gameStatArr[0];

			for (int i = 0; i < gameReps; ++i)
			{
				total += gameStatArr[i];
				printf_s("game %d, turns:%d\n", i, gameStatArr[i]);

				if (gameStatArr[i] > max)
					max = gameStatArr[i];
				if (gameStatArr[i] < min)
					min = gameStatArr[i];
			}
			printf_s("Average Turns:%f, Max Turns:%d, Min Turns:%d\n",
				(double)((double)total / (double)gameReps), max, min);
		}
		else
		{
			gameManager.StartGame(false);
		}

		fflush(stdin);
		printf_s("Play again? 'y', 'n'\n");
		answer = (char)getchar();

	} while (answer == 'y' || answer == 'Y');

}
bool RunNetworkGame()
{

	Network network;
	PacketType type;
	ErrorType error;

	/*
	** 네트워크 초기화
	*/
	try
	{
		Network::Initialize();
	}
	catch (Network::Exception)
	{
		puts("초기화 도중 문제가 발생했습니다.");
		return false;
	}

	/*
	** 서버에 연결
	서버의 IP와 포트는 당일날 공지된다.
	*/

	try
	{
		network.Connect(IP, PORT);
	}
	catch (Network::Exception ex)
	{
		switch (ex)
		{
		case Network::NETWORK_ERROR:
			puts("서버와 연결에 실패했습니다.");
			break;
		case Network::PARAMETER_ERROR:
			puts("함수의 인수가 잘못되었습니다.");
			break;
		}
		return false;
	}
	puts("접속 성공!");

	try	// 예외 처리를 위해 try문으로 모두 감싼다.
	{
		/*
		** 이름&학번 전송
		최대 길이는 MAX_NAME_LEN-1 == 15글자.
		성공시 ET_OK가 리턴된다.
		이미 있는 이름을 쓰면 ET_DUPLICATED_NAME이 온다.
		*/
		const wchar_t name[MAX_NAME_LEN] = L"김영하";
		const int studentID = 141020;

		error = network.SubmitName(name, studentID);
		if (error == ET_DUPLICATED_NAME)
		{
			puts("이미 존재하는 이름입니다.");
			return false;
		}

		/*
		** 게임 시작 대기
		게임이 시작되면 상대방의 정보가 들어온다.
		Network::GameStartData 구조체로 상대방 정보를 가져온다.
		*/
		Network::GameStartData gameStartData;
		puts("게임 시작 대기중");
		network.WaitForStart(&gameStartData);
		wprintf_s(L"매칭되었습니다. 상대방 이름: %s, 학번: %d\n", gameStartData.oppositionName, gameStartData.oppositionStudentID);

		/*
		** 게임 시작
		맵 제출부터 게임 종료까지 n회 반복한다.
		하나의 게임이 끝나고 다음 게임을 시작해야 한다면 PKT_SC_NEXT_GAME 패킷이
		모든 게임이 끝나면 PKT_SC_ALL_OVER 패킷이 들어온다.
		*/
		bool allOver = false;
		while (!allOver)
		{
			// 자신의 초기화 함수를 호출한다.
			Player myPlayer(MAP_WIDTH, MAP_HEIGHT);
			myPlayer.SetPlayerName("PeterKimAI");
			myPlayer.SetPlayerType(COMPUTER_AI);
			myPlayer.SetupShips(true);

			/*
			** 맵 제출
			자신이 배치한 맵 데이터를 서버로 전송한다.
			맵 데이터를 만드는 방법에는 두 가지가 있다.
			하나는 직접 MAP_WIDHT * MAP_HEIGHT 크기의 맵을 만드는 것이고,
			하나는 ShipData 구조체를 이용해서 만드는 것이다.
			*/
			char mapData[MAP_SIZE];
			for (int i = 0; i < MAP_SIZE; i++)
			{
				mapData[i] = 0;
			}
			myPlayer.CopyBoardDataIntoArray(mapData, MAP_SIZE);

			error = network.SubmitMap(mapData);
			if (error == ET_INVALID_MAP)
				puts("유효하지 않은 맵 데이터입니다.");
				
			

			/*
			** 게임 루프
			내 차례라면 공격 위치를 전송한다.
			차례가 끝나면 공격자와 공격 위치, 공격 결과를 받는다.
			한 게임이 끝나면 PKT_SC_GAME_OVER 패킷이 들어온다.
			*/
			bool gameOver = false;
			while (!gameOver)
			{
				error = network.GetPacketType(&type);

				switch (type)
				{
					// 에러가 발생하는 경우(상대방의 접속 종료)
				case PKT_SC_ERROR:
					if (error == ET_OPPOSITION_DISCONNECTED)
						puts("상대방의 접속이 끊어졌습니다.");
					else
						puts("알 수 없는 에러입니다.");
					return false;

					// 내 차례
				case PKT_SC_MY_TURN:
				{
					/*
					** 공격 위치 전송
					x, y는 0~7 사이의 정수이다.
					*/
					while (true)
					{
						// 자신의 공격 위치 제작 함수를 사용한다.
						Position attackPos = myPlayer.Attack();
						Coord pos;
						pos.mX = attackPos.m_X;
						pos.mY = attackPos.m_Y;

						error = network.SubmitAttack(pos);
						if (error == ET_INVALID_ATTACK)
							puts("유효하지 않은 공격 위치입니다.");
						else
							break;
					}
					break;
				}

					// 공격 결과
				case PKT_SC_ATTACK_RESULT:
				{
					Network::AttackResultData attackResult = network.GetAttackResult();
					if (attackResult.isMine)
					{
						if (SHOW_NETWORK_MSG)
							puts("공격 결과:");

						// 자신의 공격 결과 처리 함수를 사용한다.
						HandleMyAttackResult(&myPlayer, attackResult.attackResult, attackResult.pos.mX, attackResult.pos.mY);
					}
					else
					{
						if (SHOW_NETWORK_MSG)
							puts("피격 결과:");

						// 자신의 공격 결과 처리 함수를 사용한다.
						HandleOpositionAttackResult(&myPlayer, attackResult.attackResult, attackResult.pos.mX, attackResult.pos.mY);
					}

					if (SHOW_NETWORK_MSG)
						printf_s("X: %d, Y: %d, RESULT: %s\n", 
						attackResult.pos.mX, attackResult.pos.mY, ATTACK_RESULT_STR[attackResult.attackResult]);

					break;
				}

					// 게임 종료
				case PKT_SC_GAME_OVER:
				{
					Network::GameResultData gameResult = network.GetGameResult();
					if (gameResult.isWinner)
						puts("승리!!!");
					else
						puts("패배...");
					printf_s("턴 수: %d\n", gameResult.turns);
					gameOver = true;
					break;
				}

				default:
					throw Network::UNEXPECTED_PACKET;
					break;
				}
			}

			myPlayer.GetMyBoard()->PrintBoard();
			myPlayer.PrintAIBoard();

			/*
			** 종료후 처리
			*/
			network.GetPacketType(&type);

			if (type == PKT_SC_NEXT_GAME)
			{
				puts("다음 게임을 준비해주세요.");
			}
			else if (type == PKT_SC_ALL_OVER)
			{
				Network::FinalResultData finalResult = network.GetFinalResult();
				puts("모두 종료");
				printf_s("승리 횟수: %d, 평균 턴 수: %.1f", finalResult.winCount, finalResult.avgTurns);

				allOver = true;
			}
			else
				throw Network::UNEXPECTED_PACKET;
		}
	}
	catch (Network::Exception ex)
	{
		switch (ex)
		{
		case Network::NETWORK_ERROR:
			puts("네트워크에 문제가 발생했습니다.");
			break;
		case Network::SERVER_CLOSED:
			puts("서버와의 연결이 끊어졌습니다.");
			break;
		case Network::PARAMETER_ERROR:
			puts("함수의 인수가 잘못되었습니다.");
			break;
		case Network::UNEXPECTED_PACKET:
			puts("서버에서 잘못된 정보가 전송되었습니다.");
			break;
		default:
			break;
		}
	}


	/*
	** 연결 종료
	참고로 소멸시에도 자동으로 Disconnect를 호출한다.
	*/
	network.Disconnect();

	return false;
}

int _tmain()
{	
	srand((unsigned int)time(NULL));
	system("mode con: lines=3000 cols=80");
	
	printf_s("Play network battleship? or offline? 'n', 'o'\n");
	char gameType = (char)getchar();
	fflush(stdin);

	if (gameType == 'n')
	{
		RunNetworkGame();
	}
	else
	{
		RunOfflineGame();
	}

	getchar();
	return 0;
}

