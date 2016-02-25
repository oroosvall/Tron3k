#include "ClusterGrenade.h"


ClusterGrenade::ClusterGrenade(glm::vec3 position, glm::vec3 direction, int pID, int bID, int tID)
{
	initValues(position, direction, pID, bID, tID);
	maxttl = ttl = 1.5;
	vel = glm::vec3(30.0f);
	damage = 0;
	type = BULLET_TYPE::CLUSTER_GRENADE;
}

ClusterGrenade::~ClusterGrenade()
{}

int ClusterGrenade::update(float dt)
{
	pos += dir * vel * dt;

	dir.y -= 0.8f * dt;

	updateWorldMat();

	ttl -= dt;
	if (ttl < 0.0f)	//Bullet is no longer alive and should be removed
		return 1;

	return 0;
}