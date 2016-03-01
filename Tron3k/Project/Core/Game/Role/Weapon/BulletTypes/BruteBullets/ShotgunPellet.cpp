#include "ShotgunPellet.h"


ShotgunPellet::ShotgunPellet(glm::vec3 position, glm::vec3 direction, int pID, int bID, int tID)
{
	type = BULLET_TYPE::SHOTGUN_PELLET;
	initValues(position, direction, pID, bID, tID);
	vel = glm::vec3(95.0f);
	damage = 5;
	maxttl = ttl = 0.4f;
}

ShotgunPellet::~ShotgunPellet()
{}

int ShotgunPellet::update(float dt)
{
	pos += dir * vel * dt;

	updateWorldMat();

	ttl -= dt;
	if (ttl <= 0)	//Bullet is no longer alive and should be removed
		return 1;

	return 0;
}