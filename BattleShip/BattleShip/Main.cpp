// Main.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//


#include "stdafx.h"

#ifdef _DEBUG

// Debug�� �����ϴ� ���
#pragma comment(lib, "BGNL_debug.lib")

#else

// Release�� �����ϴ� ���
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
const char*				IP = "10.73.42.117"; // "10.73.42.117";	// �ڱ� ��ǻ�Ϳ��� ���� ���� 127.0.0.1
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

	// �� ������ġ�� ������ ���� ����� ���
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
	** ��Ʈ��ũ �ʱ�ȭ
	*/
	try
	{
		Network::Initialize();
	}
	catch (Network::Exception)
	{
		puts("�ʱ�ȭ ���� ������ �߻��߽��ϴ�.");
		return false;
	}

	/*
	** ������ ����
	������ IP�� ��Ʈ�� ���ϳ� �����ȴ�.
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
			puts("������ ���ῡ �����߽��ϴ�.");
			break;
		case Network::PARAMETER_ERROR:
			puts("�Լ��� �μ��� �߸��Ǿ����ϴ�.");
			break;
		}
		return false;
	}
	puts("���� ����!");

	try	// ���� ó���� ���� try������ ��� ���Ѵ�.
	{
		/*
		** �̸�&�й� ����
		�ִ� ���̴� MAX_NAME_LEN-1 == 15����.
		������ ET_OK�� ���ϵȴ�.
		�̹� �ִ� �̸��� ���� ET_DUPLICATED_NAME�� �´�.
		*/
		const wchar_t name[MAX_NAME_LEN] = L"�迵��";
		const int studentID = 141020;

		error = network.SubmitName(name, studentID);
		if (error == ET_DUPLICATED_NAME)
		{
			puts("�̹� �����ϴ� �̸��Դϴ�.");
			return false;
		}

		/*
		** ���� ���� ���
		������ ���۵Ǹ� ������ ������ ���´�.
		Network::GameStartData ����ü�� ���� ������ �����´�.
		*/
		Network::GameStartData gameStartData;
		puts("���� ���� �����");
		network.WaitForStart(&gameStartData);
		wprintf_s(L"��Ī�Ǿ����ϴ�. ���� �̸�: %s, �й�: %d\n", gameStartData.oppositionName, gameStartData.oppositionStudentID);

		/*
		** ���� ����
		�� ������� ���� ������� nȸ �ݺ��Ѵ�.
		�ϳ��� ������ ������ ���� ������ �����ؾ� �Ѵٸ� PKT_SC_NEXT_GAME ��Ŷ��
		��� ������ ������ PKT_SC_ALL_OVER ��Ŷ�� ���´�.
		*/
		bool allOver = false;
		while (!allOver)
		{
			// �ڽ��� �ʱ�ȭ �Լ��� ȣ���Ѵ�.
			Player myPlayer(MAP_WIDTH, MAP_HEIGHT);
			myPlayer.SetPlayerName("PeterKimAI");
			myPlayer.SetPlayerType(COMPUTER_AI);
			myPlayer.SetupShips(true);

			/*
			** �� ����
			�ڽ��� ��ġ�� �� �����͸� ������ �����Ѵ�.
			�� �����͸� ����� ������� �� ������ �ִ�.
			�ϳ��� ���� MAP_WIDHT * MAP_HEIGHT ũ���� ���� ����� ���̰�,
			�ϳ��� ShipData ����ü�� �̿��ؼ� ����� ���̴�.
			*/
			char mapData[MAP_SIZE];
			for (int i = 0; i < MAP_SIZE; i++)
			{
				mapData[i] = 0;
			}
			myPlayer.CopyBoardDataIntoArray(mapData, MAP_SIZE);

			error = network.SubmitMap(mapData);
			if (error == ET_INVALID_MAP)
				puts("��ȿ���� ���� �� �������Դϴ�.");
				
			

			/*
			** ���� ����
			�� ���ʶ�� ���� ��ġ�� �����Ѵ�.
			���ʰ� ������ �����ڿ� ���� ��ġ, ���� ����� �޴´�.
			�� ������ ������ PKT_SC_GAME_OVER ��Ŷ�� ���´�.
			*/
			bool gameOver = false;
			while (!gameOver)
			{
				error = network.GetPacketType(&type);

				switch (type)
				{
					// ������ �߻��ϴ� ���(������ ���� ����)
				case PKT_SC_ERROR:
					if (error == ET_OPPOSITION_DISCONNECTED)
						puts("������ ������ ���������ϴ�.");
					else
						puts("�� �� ���� �����Դϴ�.");
					return false;

					// �� ����
				case PKT_SC_MY_TURN:
				{
					/*
					** ���� ��ġ ����
					x, y�� 0~7 ������ �����̴�.
					*/
					while (true)
					{
						// �ڽ��� ���� ��ġ ���� �Լ��� ����Ѵ�.
						Position attackPos = myPlayer.Attack();
						Coord pos;
						pos.mX = attackPos.m_X;
						pos.mY = attackPos.m_Y;

						error = network.SubmitAttack(pos);
						if (error == ET_INVALID_ATTACK)
							puts("��ȿ���� ���� ���� ��ġ�Դϴ�.");
						else
							break;
					}
					break;
				}

					// ���� ���
				case PKT_SC_ATTACK_RESULT:
				{
					Network::AttackResultData attackResult = network.GetAttackResult();
					if (attackResult.isMine)
					{
						if (SHOW_NETWORK_MSG)
							puts("���� ���:");

						// �ڽ��� ���� ��� ó�� �Լ��� ����Ѵ�.
						HandleMyAttackResult(&myPlayer, attackResult.attackResult, attackResult.pos.mX, attackResult.pos.mY);
					}
					else
					{
						if (SHOW_NETWORK_MSG)
							puts("�ǰ� ���:");

						// �ڽ��� ���� ��� ó�� �Լ��� ����Ѵ�.
						HandleOpositionAttackResult(&myPlayer, attackResult.attackResult, attackResult.pos.mX, attackResult.pos.mY);
					}

					if (SHOW_NETWORK_MSG)
						printf_s("X: %d, Y: %d, RESULT: %s\n", 
						attackResult.pos.mX, attackResult.pos.mY, ATTACK_RESULT_STR[attackResult.attackResult]);

					break;
				}

					// ���� ����
				case PKT_SC_GAME_OVER:
				{
					Network::GameResultData gameResult = network.GetGameResult();
					if (gameResult.isWinner)
						puts("�¸�!!!");
					else
						puts("�й�...");
					printf_s("�� ��: %d\n", gameResult.turns);
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
			** ������ ó��
			*/
			network.GetPacketType(&type);

			if (type == PKT_SC_NEXT_GAME)
			{
				puts("���� ������ �غ����ּ���.");
			}
			else if (type == PKT_SC_ALL_OVER)
			{
				Network::FinalResultData finalResult = network.GetFinalResult();
				puts("��� ����");
				printf_s("�¸� Ƚ��: %d, ��� �� ��: %.1f", finalResult.winCount, finalResult.avgTurns);

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
			puts("��Ʈ��ũ�� ������ �߻��߽��ϴ�.");
			break;
		case Network::SERVER_CLOSED:
			puts("�������� ������ ���������ϴ�.");
			break;
		case Network::PARAMETER_ERROR:
			puts("�Լ��� �μ��� �߸��Ǿ����ϴ�.");
			break;
		case Network::UNEXPECTED_PACKET:
			puts("�������� �߸��� ������ ���۵Ǿ����ϴ�.");
			break;
		default:
			break;
		}
	}


	/*
	** ���� ����
	����� �Ҹ�ÿ��� �ڵ����� Disconnect�� ȣ���Ѵ�.
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

