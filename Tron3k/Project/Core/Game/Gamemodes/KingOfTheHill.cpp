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

	teamOneScore = 0;
	teamTwoScore = 0;
	winScore = 2;

	consolePtr = cptr;
	gamePtr = gptr;
	overtime = false;
	started = false;
	ended = false;
	teamOneSpawnTokens = 1;
	teamTwoSpawnTokens = 1;

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
	timerModifierForCaptureScoring += tickForCaptureScoring;
}

GAMEMODE_MSG KingOfTheHill::roundScoring()
{
	/*
		Vinnande laget definieras av.
		Ett lag dör.Dör ena laget utan spawn tokens vinner andra laget.
		Spawn tokens kvar.Har ett lag spawn tokens kvar vinner detta laget.
	*/
	ended = true;

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
	GAMEMODE_MSG msg = GAMEMODE_MSG::NIL;
	if (started && !ended)
	{
		if (!overtime) //Game mode proceeds as normal
		{
			timer += dt;
			if (timer - timerModifierForCaptureScoring > 0.0f) //15 seconds have passed and we should now proceed with scoring for capture point control
			{
				capturePointScoring();
				if (GetSoundActivated() && this->gamePtr->GetGameState() != Gamestate::SERVER)
				{
					GetSound()->playUserGeneratedSound(SOUNDS::soundEffectCaptureScored);
				}
			}
			if (teamOneSpawnTokens == 0 || teamTwoSpawnTokens == 0)
			{
				consolePtr->printMsg("OVERTIME BEGINS", "System", '[S]');
				overtime = true;
				timer = 30.0f;

				if (GetSoundActivated() && !overtimePlayed && this->gamePtr->GetGameState() != Gamestate::SERVER)
				{
					GetSound()->playUserGeneratedSound(SOUNDS::announcerCommence);
					GetSound()->playUserGeneratedSound(SOUNDS::SoundForOvertime);
					overtimePlayed = true;
				}
			}

			if (!this->fiveTokensPlayed && (teamOneSpawnTokens == 5 || teamTwoSpawnTokens == 5) && this->gamePtr->GetGameState() != Gamestate::SERVER)
			{
				GetSound()->playUserGeneratedSound(SOUNDS::announcer5Tokens);
				this->fiveTokensPlayed = true;
			}

		}
		else //Time down until round ends
		{
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
				if (allDead && !ended)
				{
					msg = GAMEMODE_MSG::ROUND_WIN_TEAM2;
					ended = true;
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
				if (allDead && !ended)
				{
					ended = true;
					msg = GAMEMODE_MSG::ROUND_WIN_TEAM1;
				}
			}
			if (timer < FLT_EPSILON)
			{
				ended = true;
				msg = roundScoring();
				timer = 1000.0f;		//TEMP
			}
			if (timer < 15.0f && !fifteenPlayed && this->gamePtr->GetGameState() != Gamestate::SERVER)
			{
				GetSound()->playUserGeneratedSound(SOUNDS::announcer15Seconds);
				fifteenPlayed = true;
			}
			else if (timer < 5.0f && !fivePlayed && this->gamePtr->GetGameState() != Gamestate::SERVER)
			{
				GetSound()->playUserGeneratedSound(SOUNDS::announcer5Seconds);
				fivePlayed = true;
			}

		}
	}
	else if (!ended) //Round has yet to begin!
	{
		if (consolePtr->getCommand() == "/start")
		{
			started = true;
			msg = GAMEMODE_MSG::ROUND_START;
		}
	}
	
	if (started && ended) //Round has ended, check if match ends, else reset round
	{
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
			

		else if (msg == GAMEMODE_MSG::ROUND_DRAW)
			consolePtr->printMsg("THE ROUND IS A DRAW", "System", '[S]');
		if (msg == GAMEMODE_MSG::MATCH_WIN_TEAM1)
			consolePtr->printMsg("TEAM ONE WINS THE MATCH", "System", '[S]');
		else if (msg == GAMEMODE_MSG::MATCH_WIN_TEAM2)
			consolePtr->printMsg("TEAM TWO WINS THE MATCH", "System", '[S]');
		else if (msg == GAMEMODE_MSG::MATCH_DRAW)
			consolePtr->printMsg("THE MATCH IS DRAW", "System", '[S]');
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

void KingOfTheHill::setGamemodeData(int respawn1, int respawn2, bool over, bool start, bool end)
{
	teamOneSpawnTokens = respawn1;
	teamTwoSpawnTokens = respawn2;
	overtime = over;
	started = start;
	ended = end;
}