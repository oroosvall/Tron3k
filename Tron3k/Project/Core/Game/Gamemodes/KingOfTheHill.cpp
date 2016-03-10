#include "KingOfTheHill.h"
#include "../Game.h"

KingOfTheHill::KingOfTheHill()
{

}

KingOfTheHill::~KingOfTheHill()
{

}

void KingOfTheHill::init(Console* cptr, Game* gptr)
{
	consolePtr = cptr;
	gamePtr = gptr;

	gameMode = GAMEMODE_TYPE::KOTH;
	lastMsg = GAMEMODE_MSG::NIL;

	if (gamePtr->GetGameState() == Gamestate::CLIENT) //We are a client
	{
		state = LOCAL;
		serverState = WARMUP;
	}
	else //We are the server
	{
		state = WARMUP;
		serverState = LOCAL;
	}

	capturePoint = 0; //Capture point IDs are currently 8 and 32! nvm, they're 0 and 1
	teamOnePlayersAtPoint = 0;
	teamTwoPlayersAtPoint = 0;

	teamOneScore = 0;
	teamTwoScore = 0;
	winScore = 5;

	tokensPerTeam = 20;
	pointsPerRound = 3;
	overtimer = 46.0f;

	tickForCaptureScoring = 15.0f;
	timerModifierForCaptureScoring = tickForCaptureScoring;
}

GAMEMODE_MSG KingOfTheHill::capturePointScoring()
{
	/*
	Var 15nde sekund kollas vem som har flest spelare i zonen.
	Lika många spelare : Båda lagen förlorar 1 token.
	1 - 2 spelare övertag : Laget med färre spelare förlorar 1 token.
	3 + spelare övertag : Laget med färre spelare förlorar 3 tokens.
	*/
	GAMEMODE_MSG ret = OBJECTIVE_DRAW;

	if (teamOnePlayersAtPoint == teamTwoPlayersAtPoint)
	{
		teamOneSpawnTokens--;
		teamTwoSpawnTokens--;
	}
	else if (teamOnePlayersAtPoint < teamTwoPlayersAtPoint)
	{
		if (teamTwoPlayersAtPoint - teamOnePlayersAtPoint >= 3)
		{
			teamOneSpawnTokens -= 3;
			if (teamOneSpawnTokens < 0)
				teamOneSpawnTokens = 0;
		}
		else
			teamOneSpawnTokens--;
		ret = OBJECTIVE_TEAM2;
	}
	else if (teamTwoPlayersAtPoint < teamOnePlayersAtPoint)
	{
		if (teamOnePlayersAtPoint - teamTwoPlayersAtPoint >= 3)
		{
			teamTwoSpawnTokens -= 3;
			if (teamTwoSpawnTokens < 0)
				teamTwoSpawnTokens = 0;
		}
		else
			teamTwoSpawnTokens--;
		ret = OBJECTIVE_TEAM1;
	}
	timerModifierForCaptureScoring += tickForCaptureScoring;
	return ret;
}

GAMEMODE_MSG KingOfTheHill::roundScoring()
{
	GAMEMODE_MSG ret = OBJECTIVE_DRAW;

	if (teamOnePlayersAtPoint == teamTwoPlayersAtPoint)
	{
		teamOneScore++;
		teamTwoScore++;
		ret = OBJECTIVE_DRAW;
	}
	else if (teamOnePlayersAtPoint < teamTwoPlayersAtPoint)
	{
		teamTwoScore++;
		ret = OBJECTIVE_TEAM2;
	}
	else if (teamTwoPlayersAtPoint < teamOnePlayersAtPoint)
	{
		teamOneScore++;
		ret = OBJECTIVE_TEAM1;
	}

	if (teamOneScore == winScore && teamTwoScore == winScore)
		ret = MATCH_DRAW;
	else if (teamOneScore == winScore)
		ret = MATCH_WIN_TEAM1;
	else if (teamTwoScore == winScore)
		ret = MATCH_WIN_TEAM2;

	return ret;
}

GAMEMODE_MSG KingOfTheHill::update(float dt)
{
	GAMEMODE_MSG msg = lastMsg;

	switch (state)
	{
		//The LOCAL state must never leave said LOCAL state. It only recieves information from the server
	case LOCAL: 
		if (serverState == PREROUND)
		{
		}
		else if (serverState == ROUND)
		{
			if (timer - timerModifierForCaptureScoring > 0.0f) //Sound for score plays
			{
				if (GetSoundActivated())
				{
					GetSound()->playUserGeneratedSound(SOUNDS::soundEffectCaptureScored, CATEGORY::Announcer);
				}
				timerModifierForCaptureScoring += tickForCaptureScoring;
			}
			
		}
		else if (serverState == OVERTIME)
		{
			/*if (!commencePlayed && GetSoundActivated())
			{
				GetSound()->playUserGeneratedSound(SOUNDS::SoundForOvertime);
				GetSound()->playUserGeneratedSound(SOUNDS::announcerCommence);
			}*/

			if (timer < 15.0f && !fifteenPlayed && GetSoundActivated())
			{
				GetSound()->playUserGeneratedSound(SOUNDS::announcer15Seconds, CATEGORY::Announcer);
				fifteenPlayed = true;
			}

			if (timer < 5.0f && !fivePlayed && GetSoundActivated())
			{
				GetSound()->playUserGeneratedSound(SOUNDS::announcer5Seconds, CATEGORY::Announcer);
				fivePlayed = true;
			}
		}
		break;

		//WARMUP is the pre-game, before the first round has begun. Once the game starts, we don't go here
	case WARMUP:
		teamOneSpawnTokens = teamTwoSpawnTokens = 0;
		if (gamePtr->nrOfPlayersReady() >= playersReadyNeeded)
		{
			timer = 15.0f; //20 seconds in the pre-round
			capturePoint = rand() % 2;
			teamOneSpawnTokens = teamTwoSpawnTokens = tokensPerTeam;
			state = PREROUND;
			clearTeams();
			round = 1;
			std::vector<int>* teamOne = gamePtr->getTeamConIds(1);
			std::vector<int>* teamTwo = gamePtr->getTeamConIds(2);
			for (int c = 0; c < teamOne->size(); c++)
			{
				teamOnePlayers.push_back(teamOne->at(c));
			}
			for (int c = 0; c < teamTwo->size(); c++)
			{
				teamTwoPlayers.push_back(teamTwo->at(c));
			}
		}
		else
			capturePoint = 255;
		break;

		//PREROUND is the period where people pick their classes and wait for the round to begin
	case PREROUND:
		msg = GAMEMODE_MSG::NIL;
		if (timer < FLT_EPSILON) //Time is up!
		{
			timer = 0.0f;
			state = ROUND;
			timerModifierForCaptureScoring = 15.0f;
			gamePtr->resetAllPickups();
		}
		else
		{
			timer -= dt;
		}
		break;

		//The ROUND proceeds. Server keeps track of spawn tokens, overtime, and win conditions
	case ROUND:
		timer += dt;
		if (timer - timerModifierForCaptureScoring > 0.0f) //15 seconds have passed and we should now proceed with scoring for capture point control
		{
			msg = capturePointScoring();
		}
		if (teamOneSpawnTokens == 0 || teamTwoSpawnTokens == 0)
		{
			consolePtr->printMsg("OVERTIME BEGINS", "System", 'S');
			state = OVERTIME;
			pointsLeftThisRound = pointsPerRound;
			timer = overtimer;
			timerModifierForCaptureScoring = overtimer - 16.0f;
		}
		break;

		//When OVERTIME begins, we count down from 30. We also check for win conditions.
	case OVERTIME:
	{
		timer -= dt;
		bool allDead = false;
		if (teamOneSpawnTokens == 0)
		{
			allDead = true;
			int pID = -1;
			for (int c = 0; c < teamOnePlayers.size() && allDead; c++)
			{
				pID = teamOnePlayers[c];
				Player* p = gamePtr->getPlayer(pID);
				if (p != nullptr)
				{
					if (p->isAlive())
					{
						allDead = false;
					}
				}
			}
			if (allDead)
			{
				teamTwoScore += pointsLeftThisRound;
				if (teamTwoScore >= winScore)
					msg = MATCH_WIN_TEAM2;
			}
		}
		if (teamTwoSpawnTokens == 0 && !allDead)
		{
			allDead = true;
			int pID = -1;
			for (int c = 0; c < teamTwoPlayers.size() && allDead; c++)
			{
				pID = teamTwoPlayers[c];
				Player* p = gamePtr->getPlayer(pID);
				if (p != nullptr)
				{
					if (p->isAlive())
					{
						allDead = false;
					}
				}
			}
			if (allDead)
			{
				teamOneScore += pointsLeftThisRound;
				if (teamOneScore >= winScore)
					msg = MATCH_WIN_TEAM1;
			}
		}

		if (timer - timerModifierForCaptureScoring < 0.0f) //15 seconds have passed and we should now proceed with scoring for capture point control
		{
			msg = roundScoring();
			pointsLeftThisRound--;
			timerModifierForCaptureScoring -= tickForCaptureScoring;
		}

		if (timer < FLT_EPSILON || msg == MATCH_DRAW || msg == MATCH_WIN_TEAM1 || msg == MATCH_WIN_TEAM2 || allDead)
		{
			round++;
			state = ENDROUND;
			timer = 4.0f;
		}
	}
	break;

		//After the ROUND ends, ENDROUND is the buffer time between end of a round and the PREROUND
	case ENDROUND:
		if (teamOneScore >= winScore && teamTwoScore != winScore)
		{
			msg = GAMEMODE_MSG::MATCH_WIN_TEAM1;
			timer = 10.0f;
			state = ENDMATCH;
		}
		else if (teamTwoScore >= winScore && teamOneScore != winScore)
		{
			msg = GAMEMODE_MSG::MATCH_WIN_TEAM2;
			timer = 10.0f;
			state = ENDMATCH;
		}
		else if (teamTwoScore >= winScore && teamOneScore >= winScore)
		{
			msg = GAMEMODE_MSG::MATCH_DRAW;
			timer = 10.0f;
			state = ENDMATCH;
		}
		else //Match has not ended, let's start another round!
		{
 			timer -= dt;
			if (timer < FLT_EPSILON)
			{
				if (round%2 == 0)
				{
					if (capturePoint == 0)
						capturePoint = 1;
					else
						capturePoint = 0;
				}
				else
					capturePoint = rand() % 2;

				teamOneSpawnTokens = teamTwoSpawnTokens = tokensPerTeam;
				state = PREROUND;
				timer = 15.0f;

				for (int c = 0; c < teamOnePlayers.size(); c++)
				{
					gamePtr->allowPlayerRespawn(teamOnePlayers[c], c % 5);
				}
				for (int c = 0; c < teamTwoPlayers.size(); c++)
				{
					gamePtr->allowPlayerRespawn(teamTwoPlayers[c], c % 5);
				}
			}
		}
		break;

		//If a winscore has been met for either team, we ENDMATCH and ask for rematch.
	case ENDMATCH:
		if (timer < FLT_EPSILON)
		{
			restartGame();
		}
		else
			timer -= dt;
		break;
	}
	
	lastMsg = msg;
	return msg;
}

int KingOfTheHill::getRoundWins(int team)
{
	if (team == 1)
		return teamOneScore;
	else if (team == 2)
		return teamTwoScore;
	return -1;
}

int KingOfTheHill::getRespawnTokens(int team)
{
	if (team == 1)
		return teamOneSpawnTokens;
	else if (team == 2)
		return teamTwoSpawnTokens;
	return -1;
}

int KingOfTheHill::getMaxTokensPerTeam()
{
	return tokensPerTeam;
}

bool KingOfTheHill::playerRespawn(int conId)
{
	if (state == WARMUP || state == PREROUND)
	{
		return true;
	}
	if (state == ENDROUND || state == ENDMATCH)
	{
		return false;
	}
	Player* p = gamePtr->getPlayer(conId);
	if (p->getTeam() == 1)
	{
		for (int c = 0; c < teamOnePlayers.size(); c++)
		{
			if (conId == teamOnePlayers[c])
			{
				if (teamOneSpawnTokens > 0)
				{
					teamOneSpawnTokens--;
					return true;
				}
				else
					return false;
			}
		}
		return false;
	}
	else if (p->getTeam() == 2)
	{
		for (int c = 0; c < teamTwoPlayers.size(); c++)
		{
			if (conId == teamTwoPlayers[c])
			{
				if (teamTwoSpawnTokens > 0)
				{
					teamTwoSpawnTokens--;
					return true;
				}
				else
					return false;
			}
		}
		return false;
	}
	return false;
}

void KingOfTheHill::setGamemodeData(int respawn1, int respawn2, int onCap1, int onCap2, int capPoint, float timer, int score1, int score2, KOTHSTATE state, GAMEMODE_MSG serverMsg)
{
	if (respawn1 == 5 && teamOneSpawnTokens !=5 && GetSoundActivated() && this->gamePtr->getPlayer(gamePtr->GetLocalPlayerId())->getTeam() == 1)
	{
		GetSound()->playUserGeneratedSound(SOUNDS::announcer5Tokens, CATEGORY::Announcer);
	}

	if (respawn2 == 5 && teamTwoSpawnTokens != 5 && GetSoundActivated() && this->gamePtr->getPlayer(gamePtr->GetLocalPlayerId())->getTeam() == 2)
	{
		GetSound()->playUserGeneratedSound(SOUNDS::announcer5Tokens, CATEGORY::Announcer);
	}

	this->timer = timer;
	teamOneSpawnTokens = respawn1;
	teamTwoSpawnTokens = respawn2;
	teamOnePlayersAtPoint = onCap1;
	teamTwoPlayersAtPoint = onCap2;
	capturePoint = capPoint;
	teamOneScore = score1;
	teamTwoScore = score2;
	if (serverState != state)
	{
		if (state == WARMUP)
		{
			if (GetSound())
			{
				GetSound()->SetFading(false);
				GetSound()->NoBackground();
			}
			gamePtr->clearAllPlayerKD();
			consolePtr->printMsg("Warmup. Type /ready to start.", "System", 'S');
			teamOneScore = 0;
			teamTwoScore = 0;
			round = 1;

			for (int c = 0; c < teamOnePlayers.size(); c++)
			{
				gamePtr->allowPlayerRespawn(teamOnePlayers[c], c % 5);
			}
			for (int c = 0; c < teamTwoPlayers.size(); c++)
			{
				gamePtr->allowPlayerRespawn(teamTwoPlayers[c], c % 5);
			}

			freeze = false;
			slowdownTime = false;
		}
		else if (state == PREROUND)
		{
			if (GetSound())
			{
				GetSound()->PlayBackground();
			}

			if (round == 1)
				gamePtr->clearAllPlayerKD();

			if (GetSound())
				GetSound()->setVolumeSound(0);

			freeze = true;
			std::vector<int>* teamOne = gamePtr->getTeamConIds(1);
			std::vector<int>* teamTwo = gamePtr->getTeamConIds(2);
			for (int c = 0; c < teamOne->size(); c++)
			{
				teamOnePlayers.push_back(teamOne->at(c));
			}
			for (int c = 0; c < teamTwo->size(); c++)
			{
				teamTwoPlayers.push_back(teamTwo->at(c));
			}
			
			for (int c = 0; c < teamOnePlayers.size(); c++)
			{
				gamePtr->allowPlayerRespawn(teamOnePlayers[c], c % 5);
			}
			for (int c = 0; c < teamTwoPlayers.size(); c++)
			{
				gamePtr->allowPlayerRespawn(teamTwoPlayers[c], c % 5);
			}

			consolePtr->printMsg("ROUND STARTS IN 15 SECONDS", "System", 'S');
			if (capturePoint == 0)
				consolePtr->printMsg("The active capture point is the parking area", "System", 'S');
			if (capturePoint == 1)
				consolePtr->printMsg("The active capture point is the market area", "System", 'S');
			timer = 15.0f;
		}
		else if (state == ROUND)
		{
			if (GetSound())
			{
				GetSound()->NoBackground();
			}
			

			gamePtr->resetAllPickups();
			if (GetSound())
			{
				GetSound()->setVolumeSound(50);
			}
			
			freeze = false;
			slowdownTime = false;
			timer = 0.0f;
			timerModifierForCaptureScoring = 15.0f;
			if (round == 1 && GetSoundActivated())
			{
				GetSound()->playUserGeneratedSound(SOUNDS::announcerRound1, CATEGORY::Announcer);
			}

			else if (round == 2 && GetSoundActivated())
			{
				GetSound()->playUserGeneratedSound(SOUNDS::announcerRound2, CATEGORY::Announcer);
			}

			else if (round == 3 && GetSoundActivated())
			{
				GetSound()->playUserGeneratedSound(SOUNDS::announcerRound3, CATEGORY::Announcer);
			}
		}
		else if (state == OVERTIME)
		{
			if (GetSound())
			{
				GetSound()->PlayBackground();
			}
			

			if (GetSoundActivated())
			{
				GetSound()->playUserGeneratedSound(SOUNDS::SoundForOvertime, CATEGORY::Effects);

				if (gamePtr->getPlayer(gamePtr->GetLocalPlayerId())->getTeam() == 1)
				{
					if (teamOneSpawnTokens == 0)
					{
						GetSound()->playUserGeneratedSound(SOUNDS::announcerFinalAssault, CATEGORY::Announcer);
					}
					else
					{
						GetSound()->playUserGeneratedSound(SOUNDS::announcerDefendTheObjective, CATEGORY::Announcer);
					}
				}

				else if (gamePtr->getPlayer(gamePtr->GetLocalPlayerId())->getTeam() == 2)
				{
					if (teamTwoSpawnTokens == 0)
					{
						GetSound()->playUserGeneratedSound(SOUNDS::announcerFinalAssault, CATEGORY::Announcer);
					}
					else
					{
						GetSound()->playUserGeneratedSound(SOUNDS::announcerDefendTheObjective, CATEGORY::Announcer);
					}
				}
			}
			timer = 30.0f;
		}
		else if (state == ENDROUND)
		{
			if (GetSound())
			{
				GetSound()->NoBackground();
			}
			fivePlayed = false;
			fifteenPlayed = false;
			round++;
			slowdownTime = true;

		}
		else if (state == ENDMATCH)
		{
			slowdownTime = true;
			if (serverMsg == GAMEMODE_MSG::MATCH_WIN_TEAM1)
			{
				consolePtr->printMsg("ALPHA WINS THE MATCH", "System", 'S');
				if (GetSoundActivated() && gamePtr->getPlayer(gamePtr->GetLocalPlayerId())->getTeam() == 1)
				{
					GetSound()->playUserGeneratedSound(SOUNDS::announcerYouWin, CATEGORY::Announcer);
				}
				else if (GetSoundActivated() && gamePtr->getPlayer(gamePtr->GetLocalPlayerId())->getTeam() == 2)
				{
					GetSound()->playUserGeneratedSound(SOUNDS::announcerYouLose, CATEGORY::Announcer);
				}
			}
			else if (serverMsg == GAMEMODE_MSG::MATCH_WIN_TEAM2)
			{
				consolePtr->printMsg("BETA WINS THE MATCH", "System", 'S');
				if (GetSoundActivated() && gamePtr->getPlayer(gamePtr->GetLocalPlayerId())->getTeam() == 1)
				{
					GetSound()->playUserGeneratedSound(SOUNDS::announcerYouLose, CATEGORY::Announcer);
				}
				else if (GetSoundActivated() && gamePtr->getPlayer(gamePtr->GetLocalPlayerId())->getTeam() == 2)
				{
					GetSound()->playUserGeneratedSound(SOUNDS::announcerYouWin, CATEGORY::Announcer);
				}
			}
			else if (serverMsg == GAMEMODE_MSG::MATCH_DRAW)
				consolePtr->printMsg("MATCH DRAW", "System", 'S');
			else
				consolePtr->printMsg("This shouldn't happen", "Adam", 'S');
		}
	}
	lastMsg = serverMsg;
	serverState = state;
}

int KingOfTheHill::getCapturePointOwner()
{
	if (teamOnePlayersAtPoint > teamTwoPlayersAtPoint)
		return 1;
	else if (teamOnePlayersAtPoint < teamTwoPlayersAtPoint)
		return 2;
	return 0;
}

int KingOfTheHill::getPlayersOnPoint(int team)
{
	if (team == 1)
		return teamOnePlayersAtPoint;
	else if (team == 2)
		return teamTwoPlayersAtPoint;
	return -1;
}

void KingOfTheHill::playerOnCapPointThisFrame(int team)
{
	if (team == 1)
		teamOnePlayersAtPoint++;
	else if (team == 2)
		teamTwoPlayersAtPoint++;
}

void KingOfTheHill::clearPlayersOnCapPoint()
{
	teamOnePlayersAtPoint = 0;
	teamTwoPlayersAtPoint = 0;
}

KOTHSTATE KingOfTheHill::getState()
{
	if (state == LOCAL)
	{
		return serverState; //We want to know what the server is up to rather than ourselves
	}
	return state;
}

bool KingOfTheHill::allowTeamChange()
{
	if (state == WARMUP || state == ENDMATCH)
		return true;
	return false;
}

bool KingOfTheHill::allowRoleChange()
{
	if (state == ROUND)
	{
		return false;
	}
		
	return true;
}

void KingOfTheHill::clearTeams()
{
	for (int i = 0; i < teamOnePlayers.size(); i++)
	{
		teamOnePlayers.pop_back();
	}
	for (int i = 0; i < teamTwoPlayers.size(); i++)
	{
		teamTwoPlayers.pop_back();
	}
}

void KingOfTheHill::restartGame()
{
	state = WARMUP;
	timer = 0.0f;
	teamOneScore = 0;
	teamTwoScore = 0;
	fiveTokensPlayed = false;
	commencePlayed = false;
	gamePtr->nrOfPlayersReadyReset();
	gamePtr->clearAllPlayerKD();

	for (int c = 0; c < teamOnePlayers.size(); c++)
	{
		gamePtr->allowPlayerRespawn(teamOnePlayers[c], c % 5);
	}
	for (int c = 0; c < teamTwoPlayers.size(); c++)
	{
		gamePtr->allowPlayerRespawn(teamTwoPlayers[c], c % 5);
	}
}

std::vector<int>* KingOfTheHill::getTeamVector(int team)
{
	if (team == 1)
		return &teamOnePlayers;
	if (team == 2)
		return &teamTwoPlayers;
	return nullptr;
}