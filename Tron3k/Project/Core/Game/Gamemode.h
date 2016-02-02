#ifndef GAMEMODE_H
#define GAMEMODE_H

#include "../Console.h"

class Game;

enum GAMEMODE_MSG
{
	NIL,
	ROUND_START,
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
	virtual ~Gamemode() {};
	virtual void init(Console* cptr, Game* gptr) = 0;
	virtual GAMEMODE_MSG update(float dt) = 0;
	GAMEMODE_TYPE getType() { return gameMode; };
	float getTimer() { return timer; };

	virtual bool allowTeamChange() { return true; };
	virtual bool allowRoleChange() { return true; };
};

#endif