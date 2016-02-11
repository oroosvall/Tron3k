#ifndef BULLET_H
#define BULLET_H

#include <glm/glm.hpp>
#include "../../GameObject.h"

#include "../../GameDataIndex.h"


class Bullet : public GameObject
{
protected:
	void updateWorldMat();
	void initValues(glm::vec3 pos, glm::vec3 dir, int pID, int bID, int tID);
	glm::vec3 pos;
	glm::vec3 dir;
	glm::vec3 vel;
	int damage;
	float ttl = 4;
	
	int teamId;
	int playerId;
	int bulletId = 0;
	int type = 0;

	int bounceCounter = 0;

public:
	virtual ~Bullet();
	int collided = 0;
	int getTeam() { return teamId; };

	void getId(int &player, int &bullet) { player = playerId; bullet = bulletId; };

	glm::vec3 getPos() { return pos; };
	glm::vec3 getDir() { return dir; };

	glm::vec3 getVel() { return vel; };
	void setVel(glm::vec3 inVel);

	void setPos(glm::vec3 newPos) { pos = newPos; };
	void setDir(glm::vec3 newdir);
	int getDamage() { return damage; };
	int getType() { return type; };

	virtual int update(float dt) = 0;
	void bounce() { bounceCounter++; };
	int getBounces() { return bounceCounter; };

};
#endif