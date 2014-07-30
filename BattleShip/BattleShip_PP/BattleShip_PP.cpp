// BattleShip_PP.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>
#include <vector>
#include "AirCraft.h"

const std::string strDestroy = "Destroy";
const std::string strHit = "Hit";
const std::string strMiss = "Miss";

int _tmain()
{
	/*std::string strinput;
	std::string strResult[] = { strHit, strMiss, strDestroy };
	int kind;
	int destroyedKind;
	std::vector<std::string> ships;
	std::vector<std::string> destroyedShips;

	ships.push_back("Aircraft");
	ships.push_back("Battleship");
	ships.push_back("Cruiser");
	ships.push_back("Destroyer");
	ships.push_back("Destroyer");
	
	srand((unsigned int)time(NULL));

	while (!ships.empty())
	{
		std::cin >> strinput;
		kind = rand() % _countof(strResult);
		std::cout << strResult[kind];

		if (strResult[kind] == strDestroy)
		{
			destroyedKind = rand() % ships.size();
			
			std::cout << " - ";
			
			destroyedShips.push_back(ships[destroyedKind]);
			ships.erase(ships.begin() + destroyedKind);

			for (unsigned int i = 0; i < destroyedShips.size(); i++)
			{
				std::cout << destroyedShips[i] << " ";
			}
		}
		std::cout << std::endl;
	}

	std::cout << "GAME OVER!" << std::endl;
	
	fflush(stdin);
	getchar();*/
	AirCraft ship;
	Position p;

	p.m_X = 'c';
	p.m_Y = '3';
	ship.AddPosition(p);
	p.m_X = 'C';
	ship.AddPosition(p);


	if (ship.HitCheck(p) == (HitResult)HIT)
	{
		printf("HIT\n");
	}

	ship.Print();

	getchar();

	return 0;
}

