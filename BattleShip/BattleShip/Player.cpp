#include "stdafx.h"
#include "Player.h"
#include <time.h>
#include <iostream>


Player::Player()
{
	m_ShipList[0] = &m_Aircraft;
	m_ShipList[1] = &m_Battleship;
	m_ShipList[2] = &m_Cruiser;
	m_ShipList[3] = &m_Destroyer[0];
	m_ShipList[4] = &m_Destroyer[1];

	m_MyBoard.SetBoardName("MyBoard");
	m_EnemyBoard.SetBoardName("Enemy's Board");
}


Player::~Player()
{
}

void Player::SetupShips()
{
	int maxHeight = m_MyBoard.GetMaxHeight();
	int maxWidth = m_MyBoard.GetMaxWidth();
	int startX = 0;
	int startY = 0;

	srand((unsigned int)time(NULL));

	for (int i = 0; i < _countof(m_ShipList); i++)
	{
		Ship *ship = m_ShipList[i];

		int maxHp = ship->GetMaxHP();
		Direction direction = (Direction)UP;


		do {
			direction = (Direction)(rand() % 4);

			startX = rand() % maxWidth;
			startY = rand() % maxHeight;
		} while (!IsValidPosition(startX, startY, maxHp, direction));

		// ¹èÄ¡
		PlaceShip(ship, startX, startY, direction);
	}

	m_MyBoard.PrintBoard();
}


void Player::PlaceShip( Ship* ship , int startX , int startY , Direction direction )
{
	
	for( int i = 0; i < ship->GetMaxHP(); i++ )
	{
		char curX = (char)('a' + startX);
		char curY = (char)('1' + startY);

		ship->AddPosition( curX , curY );
		m_MyBoard.AddPosition(startX, startY, (int)(ship->GetShipType()));
		
		switch (direction)
		{
			case UP:	startY--;	break;
			case DOWN:	startY++;	break;
			case LEFT:	startX--;	break;
			case RIGHT:	startX++;	break;
		}
	}
}

bool Player::IsValidPosition(int startX, int startY, int maxHp, Direction direction)
{
	//coordinates start at 0,0 at the upper left	
	for (int i = 0; i < maxHp; i++)
	{
		if (!m_MyBoard.MapCheck(startX, startY))
		{
			return false;
		}

		if (m_MyBoard.IsShipHere(startX, startY))
		{
			//printf_s("\nPos %d, %d alreadys exists!\n", startX, startY);
			return false;
		}

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

void Player::PrintShips()
{
	m_Aircraft.Print();
	m_Battleship.Print();
	m_Cruiser.Print();
	m_Destroyer[0].Print();
	m_Destroyer[1].Print();
}
