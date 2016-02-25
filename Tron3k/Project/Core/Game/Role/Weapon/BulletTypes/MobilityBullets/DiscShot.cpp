#include "DiscShot.h"


DiscShot::DiscShot(glm::vec3 position, glm::vec3 direction, int pID, int bID, int tID)
{
	initValues(position, direction, pID, bID, tID);
	vel = glm::vec3(55.0f);
	damage = 24;
	maxttl = ttl = 2.5f;
	type = BULLET_TYPE::DISC_SHOT;
}

DiscShot::~DiscShot()
{}

int DiscShot::update(float dt)
{
	pos += dir * vel * dt;

	updateWorldMat();

	ttl -= dt;
	if (ttl <= 0)	//Bullet is no longer alive and should be removed
		return 1;

	return 0;
}