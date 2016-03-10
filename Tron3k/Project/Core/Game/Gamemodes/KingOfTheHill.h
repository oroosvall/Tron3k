#ifndef KINGOFTHEHILL_H
#define KINGOFTHEHILL_H

#include "../Gamemode.h"
#include <vector>

enum KOTHSTATE
{
	LOCAL,
	WARMUP,
	PREROUND,
	ROUND,
	OVERTIME,
	ENDROUND,
	ENDMATCH
};

class KingOfTheHill : public Gamemode
{
private:
	Console* consolePtr;
	Game* gamePtr;

	KOTHSTATE state; //State on the process running
	KOTHSTATE serverState; //State of the server, if we're a client

	int playersReadyNeeded = 5;
	std::vector<int> teamOnePlayers;
	std::vector<int> teamTwoPlayers;

	int capturePoint; //Which cap point is active
	int teamOnePlayersAtPoint;
	int teamTwoPlayersAtPoint; //Number of people of each team currently in the capture point area
	int round = 1;
	int pointsLeftThisRound;

	int teamOneScore;
	int teamTwoScore;
	int winScore;

	int pointsPerRound;
	int tokensPerTeam;
	int teamOneSpawnTokens;
	int teamTwoSpawnTokens;
	float overtimer;

	bool freeze = false;
	bool slowdownTime = false;

	//For managing announcer
	bool fiveTokensPlayed = false;
	bool fifteenPlayed = false;
	bool fivePlayed = false;
	bool commencePlayed = false;

	float tickForCaptureScoring; //Used to keep track of when scoring should take place
	float timerModifierForCaptureScoring; //Used to keep track of when scoring should take place
	GAMEMODE_MSG capturePointScoring(); //Counts people at the cap point and removes tokens according to the rules

	GAMEMODE_MSG roundScoring(); //At the end of a round, this declares the winner
public:
	KingOfTheHill();
	~KingOfTheHill();
	void init(Console* cptr, Game* gptr);
	GAMEMODE_MSG update(float dt);

	void setReadyNeeded(int r) { playersReadyNeeded = r; };
	void setTeamTokens(int s) { tokensPerTeam = s; };
	void setWinScore(int w) { winScore = w; };
	void setScorePerRound(int p) { pointsPerRound = p; overtimer = float(p)*15.0f + 1.0f; };

	int getMaxTokensPerTeam();
	int getRespawnTokens(int team);
	int getRoundWins(int team);

	bool playerRespawn(int conId); //Checks if a given player is allowed to respawn

	void setGamemodeData(int respawn1, int respawn2, int onCap1, int onCap2, int capPoint, float timer, int score1, int score2, KOTHSTATE state, GAMEMODE_MSG serverMsg); //Used to keep clients updated over network

	int getCapturePoint() { return capturePoint; }; //Returns currently active cap point
	int getCapturePointOwner();
	int getPlayersOnPoint(int team); //Number of people on the point this frame (must be run after collision has been run that frame)
	void playerOnCapPointThisFrame(int team); //When Game detects a player within the cap zone, it signals with this

	KOTHSTATE getState();
	bool getIfSlowdown() { return slowdownTime; };
	bool getFreeze() { return this->freeze; }

	bool allowTeamChange();
	bool allowRoleChange();

	void clearPlayersOnCapPoint(); //Clear the values at the start of each frame
	void clearTeams();

	void restartGame();
	
	std::vector<int>* getTeamVector(int team);
};

#endif