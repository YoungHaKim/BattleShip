#pragma once
#include "stdafx.h"
#include "Board.h"
#include "AirCraft.h"
#include "BattleShip.h"
#include "Cruiser.h"
#include "Destroyer.h"
#include "Enums.h"
#include "Position.h"

class AI;

class Player
{
public:
	Player(int boardWidth, int boardHeight);
	~Player();

	Board*			GetMyBoard() { return m_MyBoard; }
	Board*			GetEnemyBoard() { return m_EnemyBoard; }

	void			SetPlayerName(std::string name);
	void			SetupShips(bool networkPlay); // random location, random direction, check if within map bounds, ships can't overlap
	void			PrintShips();
	void			PrintAIBoard();
	void			SetEnemyBoard(Board* enemyBoard);
	void			ProcessHitResult(HitResult hit);
	void			SetPlayerType(PlayerType playerType);
	void			SetAILogic(AttackLogic attackLogic);
	void			CopyBoardDataIntoArray(char *mapData, int mapSize); // mapData is OUT parameter
	bool			IsAllSunk();

	Position		Attack();
	HitResult		DoHitCheck(Position pos);
	std::string		GetPlayerName();
	

protected:
	void			PlaceShip(Ship* ship, int startX, int startY, Direction direction);
	bool			IsValidShipPosition(int startX, int startY, int maxHp, Direction direction);

private:
	int					m_Type;
	
	std::vector<Ship*>	m_ShipList;
	std::string			m_PlayerName;

	Board*				m_MyBoard;
	Board*				m_EnemyBoard;
	AI*					m_AI;
	PlayerType			m_PlayerType;
	Coordinate			m_RecentAttackCoord;
};

