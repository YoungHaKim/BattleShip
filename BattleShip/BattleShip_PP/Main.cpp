// Main.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include "GameManager.h"

int _tmain()
{	
	srand((unsigned int)time(NULL));
	system("mode con: lines=34 cols=60");

	GameManager gameManager;
	gameManager.StartGame();

	getchar();
	return 0;
}

