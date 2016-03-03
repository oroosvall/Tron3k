#ifndef BULLET_H
#define BULLET_H

#include <glm/glm.hpp>
#include "../../GameObject.h"

#include "../../GameDataIndex.h"
#include "../../ParticleStruct.h"

class Bullet : public GameObject
{
protected:
	void updateWorldMat();
	void initValues(glm::vec3 pos, glm::vec3 dir, int pID, int bID, int tID);
	glm::vec3 pos;
	glm::vec3 dir;
	glm::vec3 vel;
	int damage;
	float ttl = 3;
	float maxttl;
	
	int teamId;
	int playerId;
	int bulletId = 0;
	int type = 0;

	int bounceCounter = 0;

	bool spawnAdditionals = true;
	bool collideframe = false;

public:
	virtual ~Bullet();
	int getTeam() { return teamId; };

	void getId(int &player, int &bullet) { player = playerId; bullet = bulletId; };

	MovableParticle* part;
	MovableParticle* part2;


	glm::vec3* getPosPtr() { return &pos; }
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

	void setSpawnAdditionals(bool s) { spawnAdditionals = s; };
	bool getSpawnAdditionals() { return spawnAdditionals; };

	bool getCollidedThisFrame() { return collideframe; }; //Used to avoid some frame-perfect shenanigans with Battery Fields
	void collidedThisFrame() { collideframe = true; };

	float getCurrTTL() { return ttl; };
	float getMaxTTL() { return maxttl; };
};
#endif