// Main.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "Player.h"


int _tmain()
{	
	Player player;
	player.SetupShips();
	player.PrintShips();

	getchar();

	return 0;
}

