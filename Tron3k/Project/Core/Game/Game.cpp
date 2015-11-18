#include "Game.h"

void Game::release()
{
	// delete code goes here
	for (int i = 0; i < max_con; i++)
	{
		if (playerList[i])
		{
			delete playerList[i];
		}
	}
	delete[]playerList;

	for (int i = 0; i < bullets.size(); i++)
	{
		if (bullets[i] != nullptr)
		{
			delete bullets[i];
		}
	}

	delete this; // yes this is safe
}

Game::Game()
{

}


void Game::init(int max_connections)
{
	max_con = max_connections;

	initPhysics();

	playerList = new Player*[max_con];
	for (int c = 0; c < max_con; c++)
		playerList[c] = nullptr;
}

void Game::initPhysics()
{
	physics = CreatePhysics();
	if (!physics->init())
	{
		//console.printMsg("Error: Pipeline could not be created!", "System", 'S');
		physics->release();
		physics = nullptr;
	}
	else
	{

		/*PipelineValues pv;
		pv.type = pv.INT2;
		pv.xy[0] = winX;
		pv.xy[1] = winY;
		if (!renderPipe->setSetting(PIPELINE_SETTINGS::VIEWPORT, pv))
		{
			console.printMsg("Error: Failed to set pipeline setting: VIEWPORT", "System", 'S');
		}*/
	}
}

void Game::update(float dt)
{
	for (int c = 0; c < max_con; c++)
	{
		if (playerList[c] != nullptr)
		{
			PLAYERMSG msg = playerList[c]->update(dt);
			if (msg == PLAYERMSG::SHOOT)
			{
				createBullet(playerList[c]);
			}
		}
	}

	for (int c = 0; c < bullets.size(); c++)
	{
		int msg = bullets[c]->update(dt);
		if (msg == 1)
		{
			delete bullets[c];
		}
	}

	checkCollision();
}

Player* Game::getPlayer(int conID)
{
	if (conID < max_con)
		return playerList[conID];
	return nullptr;
}

std::vector<Bullet*> Game::getBullets()
{
	return bullets;
}

void Game::createPlayer(Player* p, int conID, bool isLocal)
{
	playerList[conID] = new Player();
	playerList[conID]->init(p->getName(), p->getPos(), isLocal);
}

void Game::removePlayer(int conID)
{
	delete playerList[conID];
	playerList[conID] = nullptr;
	removeConIDfromTeams(conID);
}

//TEMPORARY
void Game::getBoatCoordsFromCore(glm::vec3 minVals, glm::vec3 maxVals)
{
	sendBoatCoordsToPhysics(minVals, maxVals);
}

void Game::sendBoatCoordsToPhysics(glm::vec3 minVals, glm::vec3 maxVals)
{
	physics->getBoatExtremes(minVals, maxVals);
}

void Game::checkCollision()
{
	//TEMPORARY
	vec3 pPos[20];
	vec3 localPPos = vec3(0, 0, 0);
	int localP = -1;
	int localPTeam = -1;
	for (int i = 0; i < max_con; i++)
	{
		if (playerList[i] != nullptr)
		{
			if (!playerList[i]->isLocal())
				pPos[i] = playerList[i]->getPos();
			else
			{
				localPPos = playerList[i]->getPos();
				localP = i;
				//localPTeam = playerList[i]->getTeam();
			}
		}
		else
		{

		}
	}

	//Checks collision between players
	//TODO: improve so it just checks between ourselves and all other players, we don't care if two other players collided
	bool collides = false;
	for (int i = 0; i < max_con; i++)
	{
		collides = false;
		if (playerList[i] != nullptr)
			collides = physics->checkPlayerCollision(localPPos, pPos[i]);

		if (collides)
		{
			//here we can do things when two objects, collide, cause now we know i and j collided.
			//int x = 0;
			//TODO: Add separated collision places for friendly vs hostile
			vec3 inBetween = localPPos - pPos[i];
			if (localPPos.x < pPos[i].x)
			{
				inBetween.x = abs(inBetween.x);
			}
			if (localPPos.y < pPos[i].y)
			{
				inBetween.y = abs(inBetween.y);
			}
			if (localPPos.z < pPos[i].z)
			{
				inBetween.z = abs(inBetween.z);
			}
			playerList[localP]->setGoalPos(pPos[localP] - inBetween);
			playerList[i]->setGoalPos(pPos[i] + inBetween);

		}

		//TODO: Add bullet collision

	}

}

void Game::createBullet(Player* p)
{
	int wpntype;
	p->getWeaponData(wpntype);
	if (wpntype == 0)
	{
		lastBulletFired = new Bullet(p->getPos(), p->getDir(), 0.1, 0);	//add to release
		//bullets.push_back(b);
		getNewBullet(); //Temporary until I can fix networking
	}
}

int Game::getPlayersOnTeam(int team)
{
	switch (team)
	{
	case 0: return teamSpectators.size();
	case 1: return teamOne.size();
	case 2: return teamTwo.size();
	}
}

void Game::removeConIDfromTeams(int conID)
{
	for (int c = 0; c < teamSpectators.size(); c++)
	{
		if (teamSpectators[c] == conID)
		{
			teamSpectators[c] = teamSpectators[teamSpectators.size() - 1];
			teamSpectators.pop_back();
			return;
		}
	}
	for (int c = 0; c < teamOne.size(); c++)
	{
		if (teamOne[c] == conID)
		{
			teamOne[c] = teamOne[teamOne.size() - 1];
			teamOne.pop_back();
			return;
		}
	}
	for (int c = 0; c < teamTwo.size(); c++)
	{
		if (teamTwo[c] == conID)
		{
			teamTwo[c] = teamTwo[teamTwo.size() - 1];
			teamTwo.pop_back();
			return;
		}
	}
}

void Game::addPlayerToTeam(int p_conID, int team)
{
	/*
	TO DO
	Create new Player when a user switches team, or simply remove the Player when joining spectators
	*/
	switch (team)
	{
	case 0:
		removeConIDfromTeams(p_conID);
		//if (playerList[p_conID] != nullptr)
		//	delete playerList[p_conID];
		teamSpectators.push_back(p_conID);
		break;
	case 1:
		removeConIDfromTeams(p_conID);
		//if (playerList[p_conID] != nullptr)
		//	delete playerList[p_conID];
		teamOne.push_back(p_conID);
		break;
	case 2:
		removeConIDfromTeams(p_conID);
		//if (playerList[p_conID] != nullptr)
		//	delete playerList[p_conID];
		teamTwo.push_back(p_conID);
		break;
	}
}

Bullet* Game::getNewBullet()
{
	Bullet* b = addBulletToList(lastBulletFired);
	delete lastBulletFired;
	return b;
}

Bullet* Game::addBulletToList(Bullet* temp)
{
	/*
	TO DO: Add logic to find appropriate Bullet vector in the future
	*/
	Bullet* b = new Bullet();
	b->teamId = temp->teamId;
	b->pos = temp->pos;
	b->direction = temp->direction;
	b->velocity = temp->velocity;
	bullets.push_back(b);
	return b;
}