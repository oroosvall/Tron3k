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
	for (int i = 0; i < max_con; i++)
	{
		if (playerList[i] != nullptr)
			pPos[i] = playerList[i]->getPos();
		else
			pPos[i] = glm::vec3(-1, -1, -1);
	}

	for (int i = 0; i < max_con; i++)
	{
		for (int j = i + 1; j < max_con; j++)
		{
			bool collides = false;
			if (playerList[i] != nullptr && playerList[j] != nullptr)
				collides = physics->checkCollision(pPos[i], pPos[j]);

			if (collides)
			{
				//here we can do things when two objects, collide, cause now we know i and j collided.
				//int x = 0;
				vec3 inBetween = pPos[i] - pPos[j];
				if (pPos[i].x < pPos[j].x)
				{
					inBetween.x = abs(inBetween.x);
				}
				if (pPos[i].y < pPos[j].y)
				{
					inBetween.y = abs(inBetween.y);
				}
				if (pPos[i].z < pPos[j].z)
				{
					inBetween.z = abs(inBetween.z);
				}
				playerList[i]->setGoalPos(pPos[i] - inBetween);
				playerList[j]->setGoalPos(pPos[j] + inBetween);
				
			}
		}
	}
	
}

void Game::createBullet(Player* p)
{
	int wpntype;
	p->getWeaponData(wpntype);
	if (wpntype == 0)
	{
		Bullet* b = new Bullet(p->getPos(), p->getDir(), 10, 0);	//add to release
		bullets.push_back(b);
	}
}