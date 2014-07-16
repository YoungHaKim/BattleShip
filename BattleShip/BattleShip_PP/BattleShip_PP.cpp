// BattleShip_PP.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <array>
#include <vector>
#include <list>

int _tmain(int argc, _TCHAR* argv[])
{
	std::string strInput, strResult;
	srand((unsigned int)time(NULL));

	std::vector<std::string> ships = { "Aircraft", "Battleship", "Battleship", "Cruiser", "Destroyer", "Destroyer" };
	std::string results[] = { "HIT!", "MISS!", "DOUBLE HIT", "DESTROY!" };

	do
	{
		std::cout << "Input attack coordinates: type q to quit" << std::endl;
		std::cin >> strInput;

		std::cout << "You typed " << strInput << std::endl;

		int resultIdx = rand() % _countof(results);
		std::cout << results[resultIdx] << std::endl;

		if (results[resultIdx] == "DESTROY!")
		{
			int destroyedShipIndex = rand() % ships.size();
			printf_s("%s was destroyed\n", ships[destroyedShipIndex].c_str());
			ships.erase(ships.begin() + destroyedShipIndex);

			if (ships.empty())
			{
				break;
			}

			printf("Ships: [");
			for (int i = 0; i < ships.size(); i++)
			{
				printf("%s | ", ships[i].c_str());
			}
			printf("]\n\n");

		}

	} while (strInput != "q");

	std::cout << "[GAME OVER!!!!!!!!] \n\n Press ENTER to continue";
	fflush(stdin);
	getchar();
	return 0;
}

