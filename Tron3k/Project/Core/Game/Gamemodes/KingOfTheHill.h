#ifndef KINGOFTHEHILL_H
#define KINGOFTHEHILL_H

#include "../Gamemode.h"

class KingOfTheHill : public Gamemode
{
private:
	Console* consolePtr;
	Game* gamePtr;

	int teamOneScore;
	int teamTwoScore;
	int winScore;

	int teamOneSpawnTokens;
	int teamTwoSpawnTokens;
	bool overtime;
	bool started;
	bool ended;
	bool fiveTokensPlayed = false;
	bool fifteenPlayed = false;
	bool fivePlayed = false;
	bool commencePlayed = false;
	bool overtimePlayed = false;

	float tickForCaptureScoring;
	float timerModifierForCaptureScoring;
	void capturePointScoring();

	GAMEMODE_MSG roundScoring();
public:
	KingOfTheHill();
	~KingOfTheHill();
	void init(Console* cptr, Game* gptr);
	GAMEMODE_MSG update(float dt);

	int getRespawnTokens(int team);
	bool getOvertime() { return overtime; };
	bool getStarted() { return started; };
	bool getEnded() { return ended; };

	bool playerRespawn(int conId);

	void setGamemodeData(int respawn1, int respawn2, bool over, bool start, bool end);
};

#endif