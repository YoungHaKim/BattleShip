#include "stdafx.h"
#include "Player.h"


Player::Player()
{
}


Player::~Player()
{
}

void Player::SetupShips()
{
	int maxHp = m_Aircraft.GetMaxHP();
	int maxHeight = myBoard.GetMaxHeight();
	int maxWidth = myBoard.GetMaxWidth();
	int startX = 0;
	int startY = 0;
	Direction direction = (Direction)UP;
	

	do {
		direction = (Direction)(rand() % 4);

		startX = rand() % maxWidth;
		startY = rand() % maxHeight;
	}
	while (IsValidPosition(startX, startY, maxHp, direction));

	// ¹èÄ¡
	PlaceShip(m_Aircraft, startx, startY, direction);
	


}

void Player::PlaceShip(m_Aircraft, startx, startY, direction)
{

}

bool Player::IsValidPosition(int startX, int startY, int maxHp, Direction direction)
{
	return true;
}

/*
for (int i = 1; i <= m_Aircraft.GetMaxHP; i++)
{
Position p;
p.m_X = 'A';
p.m_Y = (char)i + '0';

m_Aircraft.AddPosition(p);
}
*/