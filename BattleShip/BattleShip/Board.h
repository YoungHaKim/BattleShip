#pragma once
#include <string>

class Board
{
public:
	Board();
	~Board();

	int GetMaxWidth() { return m_Width; }
	int GetMaxHeight() { return m_Height; }
	void SetBoardName(std::string name) { m_Name = name; }

private:
	int m_Width;
	int m_Height;
	int** m_Board;
	std::string m_Name;

public:
	void DrawBoard();
	bool MapCheck( int posX , int posY );
	void AddPosition(int x, int y, int value);
	void PrintBoard();
	bool IsShipHere(int x, int y);
};

