#ifndef GAME_H
#define GAME_H

#include "Player.h"
#include "../../../Physics/Physics.h"
#include <vector>

class Game
{
private:
	
	std::vector<Bullet*> bullets;

	int max_con;
	Player** playerList;
	Physics* physics;

	void initPhysics();

	void createBullet(Player* p);

public:

	Game();
	void release();
	void init(int max_connections);

	Player* getPlayer(int conID);
	std::vector<Bullet*> getBullets();
	void createPlayer(Player* p, int conID, bool isLocal = false);

	void update(float dt);

	//TEMPORARY
	void getBoatCoordsFromCore(glm::vec3 minVals, glm::vec3 maxVals);
	//TEMPORARY
	void sendBoatCoordsToPhysics(glm::vec3 minVals, glm::vec3 maxVals);
	void checkCollision();
};

#endif