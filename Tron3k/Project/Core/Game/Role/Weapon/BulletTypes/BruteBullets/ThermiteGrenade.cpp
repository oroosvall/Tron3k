#include "ThermiteGrenade.h"


ThermiteGrenade::ThermiteGrenade(glm::vec3 position, glm::vec3 direction, int pID, int bID, int tID)
{
	type = BULLET_TYPE::THERMITE_GRENADE;
	initValues(position, direction, pID, bID, tID);
	maxttl = ttl = 3;
	vel = glm::vec3(40.0f);
	damage = 10;
}

int ThermiteGrenade::update(float dt)
{
	pos += dir * vel * dt;

	dir.y -= 1.2f * dt;

	updateWorldMat();

	ttl -= dt;
	if (ttl <= 0)	//Bullet is no longer alive and should be removed
		return 1;

	return 0;
}