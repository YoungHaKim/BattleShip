#include "stdafx.h"
#include "Player.h"
#include <time.h>
#include <iostream>
#include "Position.h"

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
	// Set initial variables
	int maxHeight = m_MyBoard.GetMaxHeight();
	int maxWidth = m_MyBoard.GetMaxWidth();
	int startX = 0;
	int startY = 0;

	// Place each ship in shiplist
	for (int i = 0; i < _countof(m_ShipList); i++)
	{
		// Get ship reference
		Ship *ship = m_ShipList[i];


		// Set intial values
		int maxHp = ship->GetMaxHP();
		Direction direction = (Direction)UP;


		// random direction & start position generation for the ship
		do {

			direction = (Direction)(rand() % 4);
			startX = rand() % maxWidth;
			startY = rand() % maxHeight;

		} while (!IsValidShipPosition(startX, startY, maxHp, direction));	// boundary check, collision check


		// ¹èÄ¡
		PlaceShip(ship, startX, startY, direction);

	} // continue for loop 

	m_MyBoard.PrintBoard();
}


void Player::PlaceShip( Ship* ship , int startX , int startY , Direction direction )
{
	// Iterate by amount of ship Max HP
	for( int i = 0; i < ship->GetMaxHP(); i++ )
	{

		// Convert integer coordinates to characters
		char curX = (char)('a' + startX);
		char curY = (char)('1' + startY);


		// add position data to ships and to board
		ship->AddPosition( curX , curY );
		m_MyBoard.AddPosition(startX, startY, (int)(ship->GetShipType()));

		
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

bool Player::IsValidShipPosition(int startX, int startY, int maxHp, Direction direction)
{
	// iterate by Max HP of ship
	for (int i = 0; i < maxHp; i++)
	{
		// Map Boundary Check
		if (!m_MyBoard.MapCheck(startX, startY))
		{
			return false;
		}


		// Ship Collision Check
		if (m_MyBoard.IsShipHere(startX, startY))
		{
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

void Player::PrintShips()
{
	m_Aircraft.Print();
	m_Battleship.Print();
	m_Cruiser.Print();
	m_Destroyer[0].Print();
	m_Destroyer[1].Print();
}

Position Player::Attack()
{
	Position pos;
	pos.m_X = 'A';
	pos.m_Y = '1';

	return pos;
}

void Player::ProcessHitResult(HitResult hit)
{
	
}

HitResult Player::DoHitCheck(Position pos)
{

	for (int i = 0; i < _countof(m_ShipList); i++)
	{
		// Get ship reference
		Ship *ship = m_ShipList[i];

		HitResult hitResult = ship->HitCheck(pos);
		
		if (hitResult != MISS)
		{
			return hitResult;
		}
	}

	return MISS;
}
