#include "ClusterGrenade.h"


ClusterGrenade::ClusterGrenade(glm::vec3 position, glm::vec3 direction, int pID, int bID, int tID)
{
	initValues(position, direction, pID, bID, tID);
	ttl = 1.5;
	vel = glm::vec3(35.0f);
	damage = 0;
}

ClusterGrenade::~ClusterGrenade()
{}

int ClusterGrenade::update(float dt)
{
	pos += dir * vel * dt;

	dir.y -= 1.2f * dt;

	updateWorldMat();

	ttl -= dt;
	if (ttl < 0.0f)	//Bullet is no longer alive and should be removed
		return 1;

	return 0;
}