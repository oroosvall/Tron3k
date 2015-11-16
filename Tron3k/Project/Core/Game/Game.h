#ifndef GAME_H
#define GAME_H

#include "Player.h"
#include "../../../Physics/Physics.h"

class Game
{
private:
	
	virtual void release();

	int max_con;
	Player** playerList;
	Physics* physics;

	void initPhysics();

public:

	Game();
	void init(int max_connections);

	Player* getPlayer(int conID);
	void createPlayer(Player* p, int conID, bool isLocal = false);

	void update(float dt);

	//TEMPORARY
	void getBoatCoordsFromCore(glm::vec3 minVals, glm::vec3 maxVals);
	//TEMPORARY
	void sendBoatCoordsToPhysics(glm::vec3 minVals, glm::vec3 maxVals);

};

#endif