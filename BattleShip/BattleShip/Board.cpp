#include "stdafx.h"
#include "Board.h"
#include "Enums.h"
#include "Position.h"

Board::Board()
{
	m_Width = 8;
	m_Height = 8;
	m_Name = "UnNamed Board";


	// Malloc the ptr to ptr
	m_Board = (int**)malloc(sizeof(int)* m_Height);


	// Malloc each ptr
	for (int i = 0; i < m_Height; i++)
	{
		*(m_Board + i) = (int*)malloc(sizeof(int)* m_Width);
	}

	// init board and set to 0
	for (int i = 0; i < m_Height; i++)
	{
		for (int j = 0; j < m_Width; j++)
		{
			m_Board[i][j] = 0;
		}
	}

	//PrintBoard();
}


Board::~Board()
{
}

bool Board::MapCheck( int posX , int posY )
{
	// Check if within map bounds
	if( posX  < 0 || posX >= m_Width || posY < 0 || posY >= m_Height )
	{
		return false;
	}
	else
	{
		return true;
	}
}

void Board::AddPosition(int x, int y, int value)
{
	// If boundary check fails, do nothing
	if (MapCheck(x, y) == false)
		return;


	// If in bounds, set flag on the map
	m_Board[x][y] = value;
}

void Board::PrintBoard()
{
	// To change colors
	HANDLE  hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);	


	// Print board title
	printf_s("\n[ BOARD : %s ]\n\n", m_Name.c_str());
	

	// print board data
	for (int i = 0; i <= m_Height; i++)
	{
		for (int j = 0; j <= m_Width; j++)
		{
			SetConsoleTextAttribute(hConsole, 7);

			if (i == 0)
			{
				SetConsoleTextAttribute(hConsole, 11);

				if (j == 0) printf_s("\t");
				else printf_s("%d\t", j);

				if (j == m_Width)
				{
					printf_s("\n");
					for (int k = 0; k <= m_Width; k++)
						printf_s("-\t");
				}
			}
			else if (j == 0)  // && i == 0
			{
				SetConsoleTextAttribute(hConsole, 11);
				printf_s("%c  |\t", 'A' - 1 + i);
			}
			else
			{
				if (m_Board[i - 1][j - 1] != 0)				
					SetConsoleTextAttribute(hConsole, 14);

				printf_s("%d\t", m_Board[i - 1][j - 1]);
			}
		}
		printf_s("\n");
	}
	printf_s("\n");

	// reset console
	SetConsoleTextAttribute(hConsole, 7);
}

bool Board::IsShipHere(int x, int y)
{
	if (MapCheck(x, y) == false) 
		return false;

	if (m_Board[x][y] == 0) 
		return false;
	else
		return true;
}

void Board::ProcessAttack(Position pos)
{
	// 좌표 변환
	pos.m_X = (char)tolower((int)pos.m_X);
	int x = pos.m_X - (int)'a';
	int y = pos.m_Y - (int)'1';

	// 맞을 경우 10을 곱한다
	int hitFlagMultiplier = HIT_FLAG_MULTIPLIER;
	int missFlag = -1;


	if (m_Board[x][y] > 0 
		&& m_Board[x][y] < hitFlagMultiplier)  // 아직 안 맞았는데 배가 있다는 뜻
	{
		m_Board[x][y] *= hitFlagMultiplier;
	}
	else if (m_Board[x][y] == 0)	// MISS를 의미
	{
		m_Board[x][y] = missFlag;
	}
}

bool Board::IsAllSunk()
{
	for (int i = 0; i < m_Height; ++i)
	{
		for (int j = 0; j < m_Width; ++j)
		{
			if (m_Board[i][j] > 0 || m_Board[i][j] < HIT_FLAG_MULTIPLIER)
				return false;
		}
	}

	return true;
}


