#ifndef BULLET_H
#define BULLET_H

#include <glm/glm.hpp>
#include "../../GameObject.h"

#include "../../GameDataIndex.h"

class Bullet : public GameObject
{
protected:
	void updateWorldMat();
	glm::vec3 pos;
	glm::vec3 dir;
	int teamId;

	int playerId;
	int bulletId = 0;
public:
	virtual ~Bullet();

	int getTeamId() { return teamId; };
	void getId(int &player, int &bullet) { player = playerId; bullet = bulletId; };

	glm::vec3 getPos() { return pos; };
	glm::vec3 getDir() { return dir; };

	virtual int update(float dt) = 0;

};
#endif