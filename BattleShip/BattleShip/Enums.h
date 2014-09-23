#pragma once

enum AttackLogic
{
	STANDARD,
	CORNERFIRST,
};
enum Direction
{
	UP,
	DOWN,
	LEFT,
	RIGHT,
};
enum GameStatus
{
	PLAYING,
	GAMEOVER,
};
enum HitResult
{
	HIT,
	MISS,
	DESTROY,

	DESTROY_AIRCRAFT,
	DESTROY_BATTLESHIP,
	DESTROY_CRUISER,
	DESTROY_DESTROYER,
};
enum PlayerType
{
	HUMAN,
	COMPUTER_RANDOM,
	COMPUTER_AI,
};
enum ShipType
{
	DESTROYER = 2,
	CRUISER = 3,
	BATTLESHIP = 4,
	AIRCRAFT = 5,
};
enum Turn
{
	ATTACKER,
	DEFENDER,
};


