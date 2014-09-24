#include "stdafx.h"
#include "Player.h"
#include "AI.h"

#define NO_PLACEMENT_LOWER_BOUND	1	// to avoid placing ships in the middle of the board
#define NO_PLACEMENT_UPPER_BOUND	6	// set a lower bound and upper bound, then no ships are placed within these rows/columns

Player::Player(int boardWidth, int boardHeight)
{
	/*m_Aircraft		= new AirCraft();
	m_Battleship	= new BattleShip();
	m_Cruiser		= new Cruiser();
	m_Destroyer		= new Destroyer();*/

	m_ShipList.reserve(5);
	m_ShipList.push_back(new AirCraft(5));
	m_ShipList.push_back(new BattleShip(4));
	m_ShipList.push_back(new Cruiser(3));
	m_ShipList.push_back(new Destroyer(2));
	m_ShipList.push_back(new Destroyer(6));

	m_MyBoard		= new Board(boardWidth, boardHeight);
	m_EnemyBoard	= nullptr;

	m_MyBoard->SetBoardName("MyBoard");	//플레이어 넘버 받아서 표시해야할 듯

	m_RecentAttackCoord.x = -1;
	m_RecentAttackCoord.y = -1;

	m_PlayerType = COMPUTER_RANDOM;
}
Player::~Player()
{
	for (auto& i : m_ShipList)
	{
		delete i;
	}
	delete m_MyBoard;

	if (m_AI && m_PlayerType == COMPUTER_AI)
		delete m_AI;
}

void Player::SetupShips(bool networkPlay)
{

	// Set initial variables
	int maxHeight = m_MyBoard->GetMaxHeight();
	int maxWidth = m_MyBoard->GetMaxWidth();
	int startX = 0;
	int startY = 0;


	std::string fileReadLine;
	std::ifstream myFile("shipPlacement.txt");
	std::vector<std::string> shipPlacementVector;

	if (networkPlay 
		&& myFile.is_open() 
		&& m_PlayerType == COMPUTER_AI)
	{
		/*
		LOAD RANDOM BATCH DATA
		*/
		/*printf_s("AI is Loading random sample data\n");
		std::ifstream myRandomFile("randomShipPlacement.txt");

		if (myRandomFile.is_open())
		{
		while (std::getline(myRandomFile, fileReadLine))
		{
		m_AI->AddRandomPlacementData(fileReadLine);
		}

		}*/

		/*
		LOAD RANDOM FOOL MAPS
		*/
		while (std::getline(myFile, fileReadLine))
		{
			shipPlacementVector.push_back(fileReadLine);
		}
		myFile.close();

		/*
				std::cout << "File opened, positions " << shipPlacementVector.size() << std::endl
				<< "Press any key to continue" << std::endl;
				fflush(stdin);
				getchar();*/

		int loadIndex = rand() % shipPlacementVector.size();

		// MAP SIZE is 64, problems with malloc
		char positionArr[BOARD_SIZE];

		printf_s("Placing ships with batch file\n");

		for (int i = 0; i < maxHeight * maxWidth; i++)
		{
			int mapValue = (int)(shipPlacementVector[loadIndex].c_str()[i] - '0');
			positionArr[i] = mapValue;

			int colIndex = i % maxWidth;
			int rowIndex = (int)(i / maxWidth);

			for (int i = 0; i < (int)m_ShipList.size(); i++)
			{
				// Get ship reference
				Ship *ship = m_ShipList[i];
				int displayFlag = ship->GetDisplayFlag();

				if (displayFlag == mapValue) // the data pertains to this ship!
				{
					char curX = (char)('a' + rowIndex);
					char curY = (char)('1' + colIndex);


					// add position data to ships and to board
					ship->AddPosition(curX, curY);
					m_MyBoard->AddPosition(rowIndex, colIndex, displayFlag);

					break;
				}
			}
		}
	}
	else
	{
		// Place each ship in shiplist
		for (int i = 0; i < (int)m_ShipList.size(); i++)
		{
			// Get ship reference
			Ship *ship = m_ShipList[i];

			//printf_s("Placing ship %d \n", ship->GetShipSize(ship->GetShipType()));

			// Set intial values
			int maxHp = ship->GetMaxHP();
			Direction direction = (Direction)UP;



			// random direction & start position generation for the ship
			do {

				//direction = (Direction)(rand() % 4);

				startX = rand() % maxWidth;
				startY = rand() % maxHeight;


				if (IsValidShipPosition(startX, startY, maxHp, UP))
				{
					direction = UP;
					break;
				}
				if (IsValidShipPosition(startX, startY, maxHp, DOWN))
				{
					direction = DOWN;
					break;
				}
				if (IsValidShipPosition(startX, startY, maxHp, LEFT))
				{
					direction = LEFT;
					break;
				}
				if (IsValidShipPosition(startX, startY, maxHp, RIGHT))
				{
					direction = RIGHT;
					break;
				}


			} while (!IsValidShipPosition(startX, startY, maxHp, direction));	// boundary check, collision check


			// 배치
			PlaceShip(ship, startX, startY, direction);

		}
	}

}
// boardArray is OUT parameter!
// mapData[x + y * MAP_WIDTH] = type; 이렇게 넣으란다 정말 욕나온다 이런 맵 배치가 어디있어! 
void Player::CopyBoardDataIntoArray(char* boardArray, int mapSize)
{
	if (boardArray == nullptr)
	{
		printf_s("Null ptr error! boardArray is null in Player.cpp\n");
		return;
	}

	int destroyerCount = 1;

	for (int i = 0; i < (int)m_ShipList.size(); i++)
	{
		// Get ship reference
		Ship *ship = m_ShipList[i];
		ShipType shipType = ship->GetShipType();
		Position* posArr = ship->GetPositionArr();
		int maxHp = ship->GetMaxHP();

		int mapFillValue = 0;

		switch (shipType)
		{
		case DESTROYER:
			if (destroyerCount == 1)
			{
				mapFillValue = 4;
				destroyerCount++;
			}
			else
			{
				mapFillValue = 5;
			}
			break;
		case CRUISER:
			mapFillValue = 3;
			break;
		case BATTLESHIP:
			mapFillValue = 2;
			break;
		case AIRCRAFT:
			mapFillValue = 1;
			break;
		default:
			break;
		}

		for (int i = 0; i < maxHp; i++)
		{
			Position pos = posArr[i];
			int row = (int)(pos.m_X - 'a');
			int col = (int)(pos.m_Y - '1');
			int maxWidth = 8;
			int arrPosition = row + maxWidth * col;

			boardArray[arrPosition] = mapFillValue;
		}
	}

}
void Player::PrintShips()
{
	for (auto& i : m_ShipList)
	{
		i->Print();
	}
}
void Player::SetEnemyBoard(Board* enemyBoard)
{
	m_EnemyBoard = enemyBoard;
}
void Player::SetAILogic(AttackLogic attackLogic)
{
	m_AI->SetAILogic(attackLogic);
}
void Player::ProcessHitResult(HitResult hit)
{		
	if (m_PlayerType == COMPUTER_AI)
	{
		m_AI->ProcessLastHitResult(hit, m_RecentAttackCoord);
	}
}
void Player::PlaceShip(Ship* ship, int startX, int startY, Direction direction)
{
	// Iterate by amount of ship Max HP
	for (int i = 0; i < ship->GetMaxHP(); i++)
	{

		// Convert integer coordinates to characters
		char curX = (char)('a' + startX);
		char curY = (char)('1' + startY);


		// add position data to ships and to board
		ship->AddPosition(curX, curY);
		m_MyBoard->AddPosition(startX, startY, ship->GetDisplayFlag());


		// increment coordinates based on direction
		switch (direction)
		{
		case UP:	startY--;	break;
		case DOWN:	startY++;	break;
		case LEFT:	startX--;	break;
		case RIGHT:	startX++;	break;
		}
	}
}

Position Player::Attack()
{
	if (m_PlayerType == COMPUTER_AI)
	{
		//m_AI->ShowAIBoard();

		Coordinate coord = m_AI->ComputeNextAttack();
		m_RecentAttackCoord = coord;

		Position pos;
		pos.m_X = (char)coord.x;
		pos.m_Y = (char)coord.y;
		return pos;
	}
	else
	{

		Position pos;
		int maxHeight = m_MyBoard->GetMaxHeight();
		int maxWidth = m_MyBoard->GetMaxWidth();

		do{
			pos.m_X = (char)(rand() % maxWidth);
			pos.m_Y = (char)(rand() % maxHeight);

		} while (!m_EnemyBoard->DuplCheck(pos.m_X, pos.m_Y));

		return pos;
	}
}
HitResult Player::DoHitCheck(Position pos)
{
	for (int i = 0; i < (int)m_ShipList.size(); i++)
	{
		HitResult hitResult = m_ShipList[i]->HitCheck(pos);
		
		if (hitResult != MISS)
			return hitResult;
	}
	return MISS;
}

bool Player::IsAllSunk()
{
	for (auto& i : m_ShipList)
	{
		if (i->GetHP() != 0)
			return false;
	}
	return true;
}
bool Player::IsValidShipPosition(int startX, int startY, int maxHp, Direction direction)
{
	// iterate by Max HP of ship
	for (int i = 0; i < maxHp; i++)
	{
		// Map Boundary Check
		if (!m_MyBoard->MapBoundaryCheck(startX, startY))
		{
			return false;
		}


		// Ship Collision Check
		if (m_MyBoard->IsShipHere(startX, startY))
		{
			return false;
		}

		// X 가 3-6 사이면서 동시에 Y 도 3-6 사이에 닿인다면 배치하지 말것
		if (maxHp != 2) // destroyer만 랜덤//Carrier 만 아무데나 두자면 5로 수정
		{
			if (startX >= NO_PLACEMENT_LOWER_BOUND && startX <= NO_PLACEMENT_UPPER_BOUND
				&& startY >= NO_PLACEMENT_LOWER_BOUND && startY <= NO_PLACEMENT_UPPER_BOUND)
				return false;
		}

		// Direction Increment
		switch (direction)
		{
		case UP:	startY--;	break;
		case DOWN:	startY++;	break;
		case LEFT:	startX--;	break;
		case RIGHT:	startX++;	break;
		}
	}

	return true;
}
void Player::SetPlayerName(std::string name)
{
	m_PlayerName = name;
	m_MyBoard->SetBoardName(m_PlayerName);
}
void Player::SetPlayerType(PlayerType playerType)
{
	if (m_MyBoard == nullptr)
	{
		printf_s("Error! setup boards before setting player type!\n");
		return;
	}

	m_PlayerType = playerType;

	if (playerType == COMPUTER_AI)
	{
		if (m_AI != nullptr)
		{
			m_AI = new AI();
			m_AI->SetUpBoards(m_MyBoard);

			for (auto ship : m_ShipList)
			{
				m_AI->AddShipToTargetList(ship);
			}
		}
	}
}
void Player::PrintAIBoard()
{
	if (m_PlayerType == COMPUTER_AI && m_AI != nullptr)
	{
		m_AI->ShowAIBoard();
	}
}
std::string	Player::GetPlayerName()
{
	return m_PlayerName;
}