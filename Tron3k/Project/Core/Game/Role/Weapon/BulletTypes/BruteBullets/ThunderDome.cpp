#include "ThunderDome.h"


ThunderDome::ThunderDome(glm::vec3 position, glm::vec3 direction, int pID, int bID, int tID)
{
	initValues(position, direction, pID, bID, tID);
	vel = 30.0f;
	damage = 0;
}

ThunderDome::~ThunderDome()
{}

int ThunderDome::update(float dt)
{
	pos += dir * vel * dt;
	dir.y -= 2.0f * dt;

	updateWorldMat();

	ttl -= dt;
	if (ttl <= 0)	//Bullet is no longer alive and should be removed
		return 1;

	return 0;
}