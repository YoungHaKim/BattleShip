#pragma once
class Board
{
public:
	Board();
	~Board();

	int GetMaxWidth() { return m_Width; }
	int GetMaxHeight() { return m_Height; }

private:
	int m_Width, m_Height;
	int** board;

public:
	void DrawBoard();
};

