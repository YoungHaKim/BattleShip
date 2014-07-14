// BattleShip_PP.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

int _tmain(int argc, _TCHAR* argv[])
{
	
	std::string strInput, strResult;
	do
	{
		std::cout << "Input attack coordinates: type q to quit" << std::endl;
		std::cin >> strInput;

		std::cout << "You typed " << strInput << std::endl;

		srand(time(NULL));
		int result = rand() % 3;

		switch (result)
		{
		case 0: strResult = "HIT!"; break;
		case 1: strResult = "MISS!"; break;
		case 2: strResult = "DESTORY!"; break;

		default:
			strResult = "MISS";
			break;
		}

		std::cout << strResult << std::endl;

	} while (strInput != "q");

	std::cout << "Press ENTER to continue";
	fflush(stdin);
	getchar();
	return 0;
}

