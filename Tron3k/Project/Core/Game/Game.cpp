#include "Game.h"

void Game::release()
{
	// delete code goes here


	delete this; // yes this is safe
}

Game::Game()
{

}

void Game::init(int max_connections)
{
	max_con = max_connections;

	playerList = new Player*[max_con];
	for (int c = 0; c < max_con; c++)
		playerList[c] = nullptr;
}

Player* Game::getPlayer(int conID)
{
	if (conID < max_con)
		return playerList[conID];
	return nullptr;
}

void Game::createPlayer(Player* p, int conID)
{
	playerList[conID] = new Player();
	playerList[conID]->init(p->getName(), p->getPos());
}