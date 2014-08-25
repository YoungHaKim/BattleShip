// Main.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "GameManager.h"


#include <windows.h>
#include <iostream>
#include <cmath>

#define PI 3.14

int _tmain()
{	
	srand((unsigned int)time(NULL));
	system("mode con: lines=54 cols=60");
		
	GameManager gameManager;
	gameManager.StartGame();

	getchar();
	return 0;
}

