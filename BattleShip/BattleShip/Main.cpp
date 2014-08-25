// Main.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
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

