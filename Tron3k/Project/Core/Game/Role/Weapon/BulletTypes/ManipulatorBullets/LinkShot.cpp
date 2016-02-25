#include "LinkShot.h"


LinkShot::LinkShot(glm::vec3 position, glm::vec3 direction, int pID, int bID, int tID)
{
	type = BULLET_TYPE::LINK_SHOT;
	initValues(position, direction, pID, bID, tID);
	vel = glm::vec3(80.0f);
	damage = 50;
}

LinkShot::~LinkShot()
{}

int LinkShot::update(float dt)
{
	pos += dir * vel * dt;

	updateWorldMat();

	ttl -= dt;
	if (ttl <= 0)	//Bullet is no longer alive and should be removed
		return 1;

	return 0;
}