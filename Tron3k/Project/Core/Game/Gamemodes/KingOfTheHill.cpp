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
	gameMode = GAMEMODE_TYPE::KOTH;

	capturePoint = 0;
	teamOnePlayersAtPoint = 0;
	teamTwoPlayersAtPoint = 0;

	teamOneScore = 0;
	teamTwoScore = 0;
	winScore = 2;

	consolePtr = cptr;
	gamePtr = gptr;
	teamOneSpawnTokens = 1;
	teamTwoSpawnTokens = 1;


	if (gamePtr->GetLocalPlayerId() == -1) //We are the server
	{
		state = WARMUP;
		serverState = LOCAL;
	}
	else //We are a local client
	{
		state = LOCAL;
		serverState = WARMUP;
	}


	tickForCaptureScoring = 15.0f;
	timerModifierForCaptureScoring = tickForCaptureScoring;
}

void KingOfTheHill::capturePointScoring()
{
	/*
	Var 15nde sekund kollas vem som har flest spelare i zonen.
	Lika många spelare : Båda lagen förlorar 1 token.
	1 - 2 spelare övertag : Laget med färre spelare förlorar 1 token.
	3 + spelare övertag : Laget med färre spelare förlorar 3 tokens.
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
		Vinnande laget definieras av.
		Ett lag dör.Dör ena laget utan spawn tokens vinner andra laget.
		Spawn tokens kvar.Har ett lag spawn tokens kvar vinner detta laget.
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
		Antal spelare på kontrollpunkten.Har ena laget fler spelare på punkten vinner detta laget.
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
		Spelare vid liv.Har ena laget fler spelare vid liv vinner detta laget.
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
		Lika! Båda lagen vinner rundan.
	*/
	teamOneScore++;
	teamTwoScore++;
	if (teamOneScore == winScore && teamTwoScore == winScore)
		return GAMEMODE_MSG::MATCH_DRAW;
	return GAMEMODE_MSG::ROUND_DRAW;

}

GAMEMODE_MSG KingOfTheHill::update(float dt)
{
	clearPlayersOnCapPoint();
	GAMEMODE_MSG msg = GAMEMODE_MSG::NIL;

	switch (state)
	{
		//The LOCAL state must never leave said LOCAL state. It only recieves information from the server
	case LOCAL: 
		if (serverState == ROUND)
		{
			timer += dt;

			/*
			if (!this->fiveTokensPlayed && teamOneSpawnTokens == 5 && this->gamePtr->getPlayer(gamePtr->GetLocalPlayerId())->getTeam() == 1)
			{
				GetSound()->playUserGeneratedSound(SOUNDS::announcer5Tokens);
				this->fiveTokensPlayed = true;
			}

			if (!this->fiveTokensPlayed && teamTwoSpawnTokens == 5 && this->gamePtr->getPlayer(gamePtr->GetLocalPlayerId())->getTeam() == 2)
			{
				GetSound()->playUserGeneratedSound(SOUNDS::announcer5Tokens);
				this->fiveTokensPlayed = true;
			}

			if (GetSoundActivated())
			{
				GetSound()->playUserGeneratedSound(SOUNDS::soundEffectCaptureScored);
			}
			
			if (GetSoundActivated() && !overtimePlayed)
			{
				GetSound()->playUserGeneratedSound(SOUNDS::announcerCommence);
				GetSound()->playUserGeneratedSound(SOUNDS::SoundForOvertime);
				overtimePlayed = true;
			}
			*/
		}
		else if (serverState == OVERTIME)
		{
			timer -= dt;
			/*		
			if (timer < 15.0f && !fifteenPlayed)
			{
				GetSound()->playUserGeneratedSound(SOUNDS::announcer15Seconds);
				fifteenPlayed = true;
			}
			else if (timer < 5.0f && !fivePlayed)
			{
				GetSound()->playUserGeneratedSound(SOUNDS::announcer5Seconds);
				fivePlayed = true;
			}
			*/
		}
		break;

		//WARMUP is the pre-game, before the first round has begun. Once the game starts, we don't go here
	case WARMUP:
		if (consolePtr->getCommand() == "/start")
		{
			timer = 20.0f; //20 seconds in the pre-round
			teamOneSpawnTokens = teamTwoSpawnTokens = 10;
			state = PREROUND;
		}
		break;

		//PREROUND is the period where people pick their classes and wait for the round to begin
	case PREROUND:
		if (timer < FLT_EPSILON) //Time is up!
		{
			timer = 0.0f;
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
			consolePtr->printMsg("OVERTIME BEGINS", "System", '[S]');
			state = OVERTIME;
			timer = 30.0f;
		}
		break;

		//When OVERTIME begins, we count down from 30. We also check for win conditions.
	case OVERTIME:
		timer -= dt;
		if (teamOneSpawnTokens == 0)
		{
			vector<int>* team1ids = gamePtr->getTeamConIds(1);
			bool allDead = true;
			int pID = -1;
			bool pIsAlive = true;
			for (int c = 0; c < team1ids->size() && allDead; c++)
			{
				pID = team1ids->at(c);
				pIsAlive = gamePtr->getPlayer(pID)->isAlive();
				if (pIsAlive)
				{
					allDead = false;
				}
			}
			if (allDead)
			{
				msg = GAMEMODE_MSG::ROUND_WIN_TEAM2;
				state = ENDROUND;
			}
		}
		else if (teamTwoSpawnTokens == 0)
		{
			vector<int>* team2ids = gamePtr->getTeamConIds(2);
			bool allDead = true;
			int pID = -1;
			bool pIsAlive = true;
			for (int c = 0; c < team2ids->size() && allDead; c++)
			{
				pID = team2ids->at(c);
				pIsAlive = gamePtr->getPlayer(pID)->isAlive();
				if (pIsAlive)
				{
					allDead = false;
				}
			}
			if (allDead)
			{
				msg = GAMEMODE_MSG::ROUND_WIN_TEAM1;
				state = ENDROUND;
			}
		}
		if (timer < FLT_EPSILON)
		{
			state = ENDROUND;
			msg = roundScoring();
			timer = 1000.0f;		//TEMP
		}
		break;

		//After the ROUND ends, ENDROUND is the buffer time between end of a round and the PREROUND
	case ENDROUND:
		/*
		if (msg == GAMEMODE_MSG::ROUND_WIN_TEAM1)
		{
			consolePtr->printMsg("TEAM ONE WINS THE ROUND", "System", '[S]');

			if (this->gamePtr->GetGameState() != Gamestate::SERVER)
			{
				if (GetSoundActivated() && this->gamePtr->getPlayer(gamePtr->GetLocalPlayerId())->getTeam() == 1)
				{
					GetSound()->playUserGeneratedSound(SOUNDS::YouWin);
				}
				else if (GetSoundActivated() && this->gamePtr->getPlayer(gamePtr->GetLocalPlayerId())->getTeam() == 2)
				{
					GetSound()->playUserGeneratedSound(SOUNDS::YouLose);
				}
			}

		}

		else if (msg == GAMEMODE_MSG::ROUND_WIN_TEAM2)
		{
			consolePtr->printMsg("TEAM TWO WINS THE ROUND", "System", '[S]');
			if (this->gamePtr->GetGameState() != Gamestate::SERVER)
			{
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
		*/
		
		break;

		//If a winscore has been met for either team, we ENDMATCH and ask for rematch.
	case ENDMATCH:
		break;
	}

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
		if (teamOneSpawnTokens > 0)
		{
			teamOneSpawnTokens--;
			return true;
		}
		else
			return false;
	}
	else if (p->getTeam() == 2)
	{
		if (teamTwoSpawnTokens > 0)
		{
			teamTwoSpawnTokens--;
			return true;
		}
		else
			return false;
	}
	return false;
}

void KingOfTheHill::setGamemodeData(int respawn1, int respawn2, int onCap1, int onCap2, KOTHSTATE state)
{
	teamOneSpawnTokens = respawn1;
	teamTwoSpawnTokens = respawn2;
	teamOnePlayersAtPoint = onCap1;
	teamTwoPlayersAtPoint = onCap2;
	if (serverState != state)
	{
		if (serverState == OVERTIME)
		{
			if (GetSoundActivated() && !overtimePlayed)
			{
				GetSound()->playUserGeneratedSound(SOUNDS::announcerCommence);
				GetSound()->playUserGeneratedSound(SOUNDS::SoundForOvertime);
				overtimePlayed = true;
			}
		}
	}
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