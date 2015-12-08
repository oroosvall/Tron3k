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
	float vel;
	int damage;
	float ttl = 8;

	int teamId;
	int playerId;
	int bulletId = 0;


public:
	virtual ~Bullet();

	int getTeamId() { return teamId; };
	void getId(int &player, int &bullet) { player = playerId; bullet = bulletId; };

	glm::vec3 getPos() { return pos; };
	glm::vec3 getDir() { return dir; };
	void setDir(glm::vec3 newdir) { dir = newdir; };
	int getDamage() { return damage; };

	virtual int update(float dt) = 0;

};
#endif