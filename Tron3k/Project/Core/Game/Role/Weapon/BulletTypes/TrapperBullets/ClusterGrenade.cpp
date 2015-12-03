#include "ClusterGrenade.h"


ClusterGrenade::ClusterGrenade(glm::vec3 position, glm::vec3 direction, int pID, int bID, int tID)
{
	initValues(position, direction, pID, bID, tID);
	vel = 25.0f;
	damage = 25;
}

ClusterGrenade::~ClusterGrenade()
{}

int ClusterGrenade::update(float dt)
{
	pos += dir * vel * dt;

	updateWorldMat();

	ttl -= dt;
	if (ttl <= 0)	//Bullet is no longer alive and should be removed
		return 1;

	return 0;
}