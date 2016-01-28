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
	bool started;

	float tickForCaptureScoring;
	float timerModifierForCaptureScoring;
	void capturePointScoring();
public:
	KingOfTheHill();
	~KingOfTheHill();
	void init(Console* cptr, Game* gptr);
	GAMEMODE_MSG update(float dt);

	int getRespawnTokens(int team);
	bool getOvertime() { return overtime; };

	bool playerRespawn(int conId);
};

#endif