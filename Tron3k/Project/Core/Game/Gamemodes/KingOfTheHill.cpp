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
	winScore = 2;

	tokensPerTeam = 5;

	tickForCaptureScoring = 15.0f;
	timerModifierForCaptureScoring = tickForCaptureScoring;
}

void KingOfTheHill::capturePointScoring()
{
	/*
	Var 15nde sekund kollas vem som har flest spelare i zonen.
	Lika m�nga spelare : B�da lagen f�rlorar 1 token.
	1 - 2 spelare �vertag : Laget med f�rre spelare f�rlorar 1 token.
	3 + spelare �vertag : Laget med f�rre spelare f�rlorar 3 tokens.
	*/
	if (teamOnePlayersAtPoint == teamTwoPlayersAtPoint)
	{
		teamOneSpawnTokens--;
		teamTwoSpawnTokens--;
	}
	else if (teamOnePlayersAtPoint < teamTwoPlayersAtPoint)
	{
		if (teamTwoPlayersAtPoint - teamOnePlayersAtPoint >= 3)
			teamOnePlayersAtPoint -= 3;
		else
			teamOnePlayersAtPoint--;
	}
	else if (teamTwoPlayersAtPoint < teamOnePlayersAtPoint)
	{
		if (teamOnePlayersAtPoint - teamTwoPlayersAtPoint >= 3)
			teamTwoPlayersAtPoint -= 3;
		else
			teamTwoPlayersAtPoint--;
	}
	timerModifierForCaptureScoring += tickForCaptureScoring;
}

GAMEMODE_MSG KingOfTheHill::roundScoring()
{
	/*
	Antal spelare p� kontrollpunkten. Har ena laget fler spelare p� punkten vinner detta laget.
	*/

	if (teamOnePlayersAtPoint < teamTwoPlayersAtPoint)
	{
		teamTwoScore++;
		if (teamTwoScore == winScore)
			return GAMEMODE_MSG::MATCH_WIN_TEAM2;
		return GAMEMODE_MSG::ROUND_WIN_TEAM2;
	}
	if (teamTwoPlayersAtPoint < teamOnePlayersAtPoint)
	{
		teamOneScore++;
		if (teamOneScore == winScore)
			return GAMEMODE_MSG::MATCH_WIN_TEAM1;
		return GAMEMODE_MSG::ROUND_WIN_TEAM1;
	}

	/*
		Vinnande laget definieras av.
		Spawn tokens kvar. Har ett lag spawn tokens kvar vinner detta laget.
	*/

	if (teamOneSpawnTokens > 0)
	{
		teamOneScore++;
		if (teamOneScore == winScore)
			return GAMEMODE_MSG::MATCH_WIN_TEAM1;
		return GAMEMODE_MSG::ROUND_WIN_TEAM1;
	}
	else if (teamTwoSpawnTokens > 0)
	{
		teamTwoScore++;
		if (teamTwoScore == winScore)
			return GAMEMODE_MSG::MATCH_WIN_TEAM2;
		return GAMEMODE_MSG::ROUND_WIN_TEAM2;
	}

	/*
		Spelare vid liv. Har ena laget fler spelare vid liv vinner detta laget.
	*/

	vector<int>* team1ids = gamePtr->getTeamConIds(1);
	int T1alive = 0;
	for (int c = 0; c < team1ids->size(); c++)
	{
		if (gamePtr->getPlayer(team1ids->at(c))->isAlive())
			T1alive++;
	}

	vector<int>* team2ids = gamePtr->getTeamConIds(2);
	int T2alive = 0;
	for (int c = 0; c < team2ids->size(); c++)
	{
		if (gamePtr->getPlayer(team2ids->at(c))->isAlive())
			T2alive++;
	}

	if (T1alive < T2alive)
	{
		teamTwoScore++;
		if (teamTwoScore == winScore)
			return GAMEMODE_MSG::MATCH_WIN_TEAM2;
		return GAMEMODE_MSG::ROUND_WIN_TEAM2;
	}
	else if (T1alive > T2alive)
	{
		teamOneScore++;
		if (teamOneScore == winScore)
			return GAMEMODE_MSG::MATCH_WIN_TEAM1;
		return GAMEMODE_MSG::ROUND_WIN_TEAM1;
	}

	/*
		Lika! B�da lagen vinner rundan.
	*/
	teamOneScore++;
	teamTwoScore++;
	if (teamOneScore == winScore && teamTwoScore == winScore)
		return GAMEMODE_MSG::MATCH_DRAW;
	return GAMEMODE_MSG::ROUND_DRAW;

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
			timer += dt;
			if (timer - timerModifierForCaptureScoring > 0.0f) //Sound for score plays
			{
				if (GetSoundActivated())
				{
					GetSound()->playUserGeneratedSound(SOUNDS::soundEffectCaptureScored);
				}
				timerModifierForCaptureScoring += tickForCaptureScoring;
			}
			
		}
		else if (serverState == OVERTIME)
		{
			timer -= dt;
		}
		break;

		//WARMUP is the pre-game, before the first round has begun. Once the game starts, we don't go here
	case WARMUP:
		if (consolePtr->getCommand() == "/start")
		{
			timer = 15.0f; //20 seconds in the pre-round
			state = PREROUND;
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
		break;

		//PREROUND is the period where people pick their classes and wait for the round to begin
	case PREROUND:
		msg = GAMEMODE_MSG::NIL;
		if (timer < FLT_EPSILON) //Time is up!
		{
			timer = 0.0f;
			for (int c = 0; c < teamOnePlayers.size(); c++)
			{
				gamePtr->allowPlayerRespawn(teamOnePlayers[c], c);
			}
			for (int c = 0; c < teamTwoPlayers.size(); c++)
			{
				gamePtr->allowPlayerRespawn(teamTwoPlayers[c], c);
			}
			gamePtr->clearAllPlayerKD();
			teamOneSpawnTokens = teamTwoSpawnTokens = tokensPerTeam;
			state = ROUND;
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
			capturePointScoring();
		}
		if (teamOneSpawnTokens == 0 || teamTwoSpawnTokens == 0)
		{
			consolePtr->printMsg("OVERTIME BEGINS", "System", 'S');
			state = OVERTIME;
			timer = 30.0f;
		}
		break;

		//When OVERTIME begins, we count down from 30. We also check for win conditions.
	case OVERTIME:
		timer -= dt;
		if (teamOneSpawnTokens == 0)
		{
			bool allDead = true;
			int pID = -1;
			bool pIsAlive = true;
			for (int c = 0; c < teamOnePlayers.size() && allDead; c++)
			{
				pID = teamOnePlayers[c];
				pIsAlive = gamePtr->getPlayer(pID)->isAlive();
				if (pIsAlive)
				{
					allDead = false;
				}
			}
			if (allDead)
			{
				msg = GAMEMODE_MSG::ROUND_WIN_TEAM2;
				teamTwoScore++;
				state = ENDROUND;
			}
		}
		else if (teamTwoSpawnTokens == 0)
		{
			bool allDead = true;
			int pID = -1;
			bool pIsAlive = true;
			for (int c = 0; c < teamTwoPlayers.size() && allDead; c++)
			{
				pID = teamTwoPlayers[c];
				pIsAlive = gamePtr->getPlayer(pID)->isAlive();
				if (pIsAlive)
				{
					allDead = false;
				}
			}
			if (allDead)
			{
				msg = GAMEMODE_MSG::ROUND_WIN_TEAM1;
				teamOneScore++;
				state = ENDROUND;
			}
		}
		if (timer < FLT_EPSILON)
		{
			state = ENDROUND;
			msg = roundScoring();
			timer = 10.0f;		//TEMP
		}
		break;

		//After the ROUND ends, ENDROUND is the buffer time between end of a round and the PREROUND
	case ENDROUND:
		if (teamOneScore == winScore)
		{
			msg = GAMEMODE_MSG::MATCH_WIN_TEAM1;
			state = ENDMATCH;
		}
		else if (teamTwoScore == winScore)
		{
			msg = GAMEMODE_MSG::MATCH_WIN_TEAM2;
			state = ENDMATCH;
		}
		else if (teamTwoScore == winScore && teamOneScore == winScore)
		{
			msg = GAMEMODE_MSG::MATCH_DRAW;
			state = ENDMATCH;
		}
		else //Match has not ended, let's start another round!
		{
			timer -= dt;
			if (timer < FLT_EPSILON)
			{
				state = PREROUND;
				timer = 10.0f;
			}
		}
		break;

		//If a winscore has been met for either team, we ENDMATCH and ask for rematch.
	case ENDMATCH:
		break;
	}
	
	lastMsg = msg;
	return msg;
}

int KingOfTheHill::getRespawnTokens(int team)
{
	if (team == 1)
		return teamOneSpawnTokens;
	else if (team == 2)
		return teamTwoSpawnTokens;
	return -1;
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

void KingOfTheHill::setGamemodeData(int respawn1, int respawn2, int onCap1, int onCap2, KOTHSTATE state, GAMEMODE_MSG serverMsg)
{
	teamOneSpawnTokens = respawn1;
	teamTwoSpawnTokens = respawn2;
	teamOnePlayersAtPoint = onCap1;
	teamTwoPlayersAtPoint = onCap2;
	if (serverState != state)
	{
		if (state == PREROUND)
		{
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
			consolePtr->printMsg("ROUND STARTS IN 15 SECONDS", "System", 'S');
		}
		else if (state == ROUND)
		{
			for (int c = 0; c < teamOnePlayers.size(); c++)
			{
				gamePtr->allowPlayerRespawn(teamOnePlayers[c], c);
			}
			for (int c = 0; c < teamTwoPlayers.size(); c++)
			{
				gamePtr->allowPlayerRespawn(teamTwoPlayers[c], c);
			}
			gamePtr->clearAllPlayerKD();
		}
		else if (state == OVERTIME)
		{
			if (GetSoundActivated())
			{
				GetSound()->playUserGeneratedSound(SOUNDS::announcerCommence);
				GetSound()->playUserGeneratedSound(SOUNDS::SoundForOvertime);
			}
		}
		else if (state == ENDROUND)
		{
			if (serverMsg == GAMEMODE_MSG::ROUND_WIN_TEAM1)
			{
				consolePtr->printMsg("TEAM ONE WINS THE ROUND", "System", 'S');
				if (GetSoundActivated() && this->gamePtr->getPlayer(gamePtr->GetLocalPlayerId())->getTeam() == 1)
				{
					GetSound()->playUserGeneratedSound(SOUNDS::YouWin);
				}
				else if (GetSoundActivated() && this->gamePtr->getPlayer(gamePtr->GetLocalPlayerId())->getTeam() == 2)
				{
					GetSound()->playUserGeneratedSound(SOUNDS::YouLose);
				}
			}

			else if (serverMsg == GAMEMODE_MSG::ROUND_WIN_TEAM2)
			{
				consolePtr->printMsg("TEAM TWO WINS THE ROUND", "System", 'S');
				if (GetSoundActivated() && this->gamePtr->getPlayer(gamePtr->GetLocalPlayerId())->getTeam() == 1)
				{
					GetSound()->playUserGeneratedSound(SOUNDS::YouLose);
				}
				else if (GetSoundActivated() && this->gamePtr->getPlayer(gamePtr->GetLocalPlayerId())->getTeam() == 2)
				{
					GetSound()->playUserGeneratedSound(SOUNDS::YouWin);
				}
			}
		}
		else if (state == ENDMATCH)
		{
			if (serverMsg == GAMEMODE_MSG::MATCH_WIN_TEAM1)
			{
				consolePtr->printMsg("TEAM ONE WINS THE MATCH", "System", 'S');
				if (GetSoundActivated() && this->gamePtr->getPlayer(gamePtr->GetLocalPlayerId())->getTeam() == 1)
				{
					GetSound()->playUserGeneratedSound(SOUNDS::announcerYouWin);
				}
				else if (GetSoundActivated() && this->gamePtr->getPlayer(gamePtr->GetLocalPlayerId())->getTeam() == 2)
				{
					GetSound()->playUserGeneratedSound(SOUNDS::announcerYouLose);
				}
			}
			else if (serverMsg == GAMEMODE_MSG::MATCH_WIN_TEAM2)
			{
				consolePtr->printMsg("TEAM TWO WINS THE MATCH", "System", 'S');
				if (GetSoundActivated() && this->gamePtr->getPlayer(gamePtr->GetLocalPlayerId())->getTeam() == 1)
				{
					GetSound()->playUserGeneratedSound(SOUNDS::announcerYouLose);
				}
				else if (GetSoundActivated() && this->gamePtr->getPlayer(gamePtr->GetLocalPlayerId())->getTeam() == 2)
				{
					GetSound()->playUserGeneratedSound(SOUNDS::announcerYouWin);
				}
			}
			else if (serverMsg == GAMEMODE_MSG::MATCH_DRAW)
				consolePtr->printMsg("TEAM TWO WINS THE MATCH", "System", 'S');
			else
				consolePtr->printMsg("This shouldn't happen", "Adam", 'S');
		}
	}
	lastMsg = serverMsg;
	serverState = state;
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
		return false;
	return true;
}