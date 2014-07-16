// BattleShip_PP.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <array>
#include <vector>
#include <list>

/*
객체 리스트

1. 배 - 5개 필요
2. 탄
3. 판 - Map class에서 4개를 생성
4. 심판 (규칙/운영)
5. 시계(타이머)
6. 플레이어 - 2명 필요
7. 점수판
8. 비주얼 - 애니메이터 
9. 사운드 이팩트

교수 포인트:
1. 플레이어 꼭 필요
2. 게임 규칙도 클래스다 (Game Manager)
	2.1. 예를 들어, 바둑판을 만들면, 바둑만 두는 것이 아니라, 알까기, 밥먹기 가능
		즉, 객체는 상황에 따라 용도가 달라질 수 있다.
		그러므로 바둑이라는 개념은 판에 따라 정해지지 않음

3. Player class가 map에다가 ship를 배치
4. Game Manager가 차례 알려주기, 게임 시작 끝 알리기

>> 개념이 중요하다
>> Paradigm을 이해해야 한다
>> 현실에 존재하는 객체를 생각하고 정의한다

객체 
-속성 (값으로 표현할 수 있는 것) > member variables
-행동 (수치로 표현되지 않는 것) > method

Game Mgr: (규칙)
- 속성: 게임 규칙, 게임 상태
- 행동: 게임 중재/운영, 배가 맞았다 안 맞았다 알려주기, 

Player:
- 속성: IQ, 정체(사람/컴퓨터/랜덤)
- 행동: 게임 하기, 게임 관두기, 배 배치하기, 맵에 표시하기, 포탄 던지기 (공격)

Map:
- 속성: 판 자체(공간), 공간이 비었나 안 비었나 (flag)
- 행동: 생성되기, 소멸되기

Ship:
- 속성: 이름(종류), 상태(살아있다, 갈아앉았다), 위치, 길이, hit miss history(맞은 위치), HitPoints (총 HP, damage)
- 행동: 가라앉기, 

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

