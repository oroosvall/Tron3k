#ifndef BULLET_H
#define BULLET_H

#include <glm/glm.hpp>
#include "../GameObject.h"

#include "../GameDataIndex.h"

class Bullet : public GameObject
{
protected:
	void updateWorldMat();
public:
	glm::vec3 pos;
	glm::vec3 dir;
	int teamId;

	virtual ~Bullet();

	virtual int update(float dt) = 0;

};
#endif