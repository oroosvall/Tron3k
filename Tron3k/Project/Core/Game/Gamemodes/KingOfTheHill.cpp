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
	Check nr of people on each team that are within active zone
	Compare
	Follow rules to determine removed respawns
	*/
	timerModifierForCaptureScoring += tickForCaptureScoring;
}

GAMEMODE_MSG KingOfTheHill::update(float dt)
{
	if (started)
	{
		if (!overtime) //Game mode proceeds as normal
		{
			timer += dt;
			if (timer - timerModifierForCaptureScoring > 0.0f) //15 seconds have passed and we should now proceed with scoring for capture point control
			{
				capturePointScoring();
			}

		}
		else //Time down until round ends
		{
			timer -= dt;
		}
	}
	else
	{
		if (consolePtr->getCommand() == "/start")
			started = true;
	}
	return GAMEMODE_MSG::NIL;
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