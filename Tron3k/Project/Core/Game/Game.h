#ifndef GAME_H
#define GAME_H

#include "Player.h"
#include "../../../Physics/Physics.h"
#include <vector>

class Game
{
private:
	
	std::vector<Bullet*> bullets;

	std::vector<int> teamSpectators; //Team vectors hold connection IDs
	std::vector<int> teamOne;
	std::vector<int> teamTwo;

	int maxTeamSize = 5;
	int maxSpec = 5;
	void removeConIDfromTeams(int conID);

	int max_con;
	Player** playerList;
	Physics* physics;

	void initPhysics();

	void createBullet(Player* p);
	Bullet* lastBulletFired;
	bool bulletReady = false;

public:

	Game();
	void release();
	void init(int max_connections);

	Player* getPlayer(int conID);
	std::vector<Bullet*> getBullets();
	void createPlayer(Player* p, int conID, bool isLocal = false);
	void removePlayer(int conID);

	void update(float dt);

	//TEMPORARY
	void getBoatCoordsFromCore(glm::vec3 minVals, glm::vec3 maxVals);
	//TEMPORARY
	void sendBoatCoordsToPhysics(glm::vec3 minVals, glm::vec3 maxVals);
	void checkCollision();

	void addPlayerToTeam(int p_conID, int team);
	int getPlayersOnTeam(int team);
	int getMaxTeamSize(bool spec = false) { if (spec) return maxSpec; return maxTeamSize; };

	void addBulletToList(Bullet* b);
	bool isBulletReady() { return bulletReady; };
	Bullet* getNewBullet();
};

#endif