#include "Game.h"

void Game::release()
{
	// delete code goes here
	for (int i = 0; i < max_con; i++)
	{
		delete playerList[i];
	}
	delete[]playerList;

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
			playerList[c]->update(dt);
	}

	checkCollision();
}

Player* Game::getPlayer(int conID)
{
	if (conID < max_con)
		return playerList[conID];
	return nullptr;
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

	physics->checkCollision(pPos, 20);
}