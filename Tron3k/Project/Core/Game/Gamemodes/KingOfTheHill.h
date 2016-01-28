#ifndef KINGOFTHEHILL_H
#define KINGOFTHEHILL_H

#include "../Gamemode.h"

class KingOfTheHill : public Gamemode
{
private:
	Console* consolePtr;
	Game* gamePtr;

	int teamOneSpawnTokens;
	int teamTwoSpawnTokens;
	bool overtime;
public:
	KingOfTheHill();
	~KingOfTheHill();
	void init(Console* cptr, Game* gptr);
	int update(float dt);

	int getRespawnTokens(int team);
	bool getOvertime() { return overtime; };

	bool playerRespawn(int conId);
};

#endif