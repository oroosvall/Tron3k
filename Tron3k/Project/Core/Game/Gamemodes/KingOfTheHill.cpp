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
	consolePtr = cptr;
	gamePtr = gptr;
	overtime = false;
	started = false;
	teamOneSpawnTokens = 20;
	teamTwoSpawnTokens = 20;

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
		Antal spelare på kontrollpunkten.Har ena laget fler spelare på punkten vinner detta laget.
		Spelare vid liv.Har ena laget fler spelare vid liv vinner detta laget.
		Lika!Båda lagen vinner rundan.

		Detta kan innebära att en match(bäst av sju rundor) KAN avslutas i lika i extrema situationer.*/
	return GAMEMODE_MSG::NIL;

}

GAMEMODE_MSG KingOfTheHill::update(float dt)
{
	GAMEMODE_MSG msg = GAMEMODE_MSG::NIL;
	if (started)
	{
		if (!overtime) //Game mode proceeds as normal
		{
			timer += dt;
			if (timer - timerModifierForCaptureScoring > 0.0f) //15 seconds have passed and we should now proceed with scoring for capture point control
			{
				capturePointScoring();
			}
			if (teamOneSpawnTokens == 0 || teamTwoSpawnTokens == 0)
			{
				overtime = true;
				timer = 30.0f;
			}

		}
		else //Time down until round ends
		{
			timer -= dt;
			if (teamOneSpawnTokens == 0)
			{
				vector<int>* team1ids = gamePtr->getTeamConIds(1);
				bool allDead = true;
				for (int c = 0; c < team1ids->size() && allDead; c++)
				{
					if (gamePtr->getPlayer(team1ids->at(c))->isAlive())
						allDead = false;
				}
				if (allDead)
				{
					msg = roundScoring();
				}
			}
			else if (teamTwoSpawnTokens == 0)
			{
				vector<int>* team2ids = gamePtr->getTeamConIds(2);
				bool allDead = true;
				for (int c = 0; c < team2ids->size() && allDead; c++)
				{
					if (gamePtr->getPlayer(team2ids->at(c))->isAlive())
						allDead = false;
				}
				if (allDead)
				{
					msg = roundScoring();
				}
			}
			else if (timer < FLT_EPSILON)
			{
				msg = roundScoring();
			}
		}
	}
	else
	{
		if (consolePtr->getCommand() == "/start")
		{
			started = true;
			teamOneSpawnTokens = 20;
			teamTwoSpawnTokens = 20;
		}
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