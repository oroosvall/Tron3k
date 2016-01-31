#ifndef KINGOFTHEHILL_H
#define KINGOFTHEHILL_H

#include "../Gamemode.h"

class KingOfTheHill : public Gamemode
{
private:
	Console* consolePtr;
	Game* gamePtr;

	int capturePoint; //Which cap point is active
	int teamOnePlayersAtPoint;
	int teamTwoPlayersAtPoint; //Number of people of each team currently in the capture point area

	int teamOneScore;
	int teamTwoScore;
	int winScore;

	int teamOneSpawnTokens;
	int teamTwoSpawnTokens;
	bool overtime; //Signals that we are in overtime
	bool started; //Signals that a round has started
	bool ended; //Signals that a round has ended

	//For managing announcer
	bool fiveTokensPlayed = false;
	bool fifteenPlayed = false;
	bool fivePlayed = false;
	bool commencePlayed = false;
	bool overtimePlayed = false;

	float tickForCaptureScoring; //Used to keep track of when scoring should take place
	float timerModifierForCaptureScoring; //Used to keep track of when scoring should take place
	void capturePointScoring(); //Counts people at the cap point and removes tokens according to the rules

	GAMEMODE_MSG roundScoring(); //At the end of a round, this declares the winner
public:
	KingOfTheHill();
	~KingOfTheHill();
	void init(Console* cptr, Game* gptr);
	GAMEMODE_MSG update(float dt);

	int getRespawnTokens(int team);
	bool getOvertime() { return overtime; };
	bool getStarted() { return started; };
	bool getEnded() { return ended; };

	bool playerRespawn(int conId); //Checks if a given player is allowed to respawn

	void setGamemodeData(int respawn1, int respawn2, bool over, bool start, bool end); //Used to keep clients updated over network

	int getCapturePoint() { return capturePoint; }; //Returns currently active cap point
	int getPlayersOnPoint(int team); //Number of people on the point this frame (must be run after collision has been run that frame)
	void playerOnCapPointThisFrame(int team); //When Game detects a player within the cap zone, it signals with this
	void clearPlayersOnCapPointAtStartOfFrame(); //Clear the values at the start of each frame
};

#endif