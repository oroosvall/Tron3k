#include "KingOfTheHill.h"

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
	teamOneSpawnTokens = 20;
	teamTwoSpawnTokens = 20;
}

int KingOfTheHill::update(float dt)
{
	if (!overtime) //Game mode proceeds as normal
	{
		timer += dt;
		
	}
	else //Time down until round ends
	{
		timer -= dt;
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