// BattleShip_PP.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <array>
#include <vector>
#include <list>

/*
��ü ����Ʈ

1. �� - 5�� �ʿ�
2. ź
3. �� - Map class���� 4���� ����
4. ���� (��Ģ/�)
5. �ð�(Ÿ�̸�)
6. �÷��̾� - 2�� �ʿ�
7. ������
8. ���־� - �ִϸ����� 
9. ���� ����Ʈ

���� ����Ʈ:
1. �÷��̾� �� �ʿ�
2. ���� ��Ģ�� Ŭ������ (Game Manager)
	2.1. ���� ���, �ٵ����� �����, �ٵϸ� �δ� ���� �ƴ϶�, �˱��, ��Ա� ����
		��, ��ü�� ��Ȳ�� ���� �뵵�� �޶��� �� �ִ�.
		�׷��Ƿ� �ٵ��̶�� ������ �ǿ� ���� �������� ����

3. Player class�� map���ٰ� ship�� ��ġ
4. Game Manager�� ���� �˷��ֱ�, ���� ���� �� �˸���

>> ������ �߿��ϴ�
>> Paradigm�� �����ؾ� �Ѵ�
>> ���ǿ� �����ϴ� ��ü�� �����ϰ� �����Ѵ�

��ü 
-�Ӽ� (������ ǥ���� �� �ִ� ��) > member variables
-�ൿ (��ġ�� ǥ������ �ʴ� ��) > method

Game Mgr: (��Ģ)
- �Ӽ�: ���� ��Ģ, ���� ����
- �ൿ: ���� ����/�, �谡 �¾Ҵ� �� �¾Ҵ� �˷��ֱ�, 

Player:
- �Ӽ�: IQ, ��ü(���/��ǻ��/����)
- �ൿ: ���� �ϱ�, ���� ���α�, �� ��ġ�ϱ�, �ʿ� ǥ���ϱ�, ��ź ������ (����)

Map:
- �Ӽ�: �� ��ü(����), ������ ����� �� ����� (flag)
- �ൿ: �����Ǳ�, �Ҹ�Ǳ�

Ship:
- �Ӽ�: �̸�(����), ����(����ִ�, ���ƾɾҴ�), ��ġ, ����, hit miss history(���� ��ġ), HitPoints (�� HP, damage)
- �ൿ: ����ɱ�, 

*/

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

