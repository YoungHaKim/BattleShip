// Main.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include "Player.h"
#include "GameManager.h"
#include <time.h>


int _tmain()
{	
	// set random seed
	srand((unsigned int)time(NULL));

	GameManager gameManager;

	gameManager.StartGame();

	
	


	getchar();

	return 0;
}

