#ifndef GAMEMODE_H
#define GAMEMODE_H

#include "Game.h"
#include "../Console.h"

enum GAMEMODE_MSG
{
	NIL,
	ROUND_WIN_TEAM1,
	ROUND_WIN_TEAM2,
	ROUND_DRAW,
	MATCH_WIN_TEAM1,
	MATCH_WIN_TEAM2,
	MATCH_DRAW
};

enum GAMEMODE_TYPE
{
	KOTH,
	TDM
};

class Gamemode
{
private:
	Console* consolePtr;
	Game* gamePtr;
protected:
	GAMEMODE_TYPE gameMode;
	float timer;
public:
	virtual ~Gamemode();
	virtual void init(Console* cptr, Game* gptr) = 0;
	virtual int update(float dt) = 0;
	GAMEMODE_TYPE getType() { return gameMode; };
	float getTimer() { return timer; };
};

#endif