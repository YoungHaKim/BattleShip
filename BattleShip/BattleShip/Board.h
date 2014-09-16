#pragma once
#include "Position.h"

#define HIT_FLAG_MULTIPLIER 10

class Board
{
public:	
	Board(int width, int height);
	~Board();

	void SetBoardName(std::string name) { m_Name = name; }
	int GetMaxWidth() { return m_Width; }
	int GetMaxHeight() { return m_Height; }

	//void DrawBoard();
	void AddPosition(int x, int y, int value);
	void PrintBoard();
	void ProcessAttack(Position pos);
	bool DuplCheck(int x, int y);
	bool MapBoundaryCheck(int posX, int posY);
	bool MapBoundaryCheck(Coordinate coordinate);
	bool IsShipHere(int x, int y);

	std::string		GetBoardAsString();

protected:
	std::string m_Name;
	int m_Width;
	int m_Height;
	int** m_Board;
};

