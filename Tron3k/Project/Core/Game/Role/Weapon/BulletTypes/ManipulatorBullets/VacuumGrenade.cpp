#include "VacuumGrenade.h"


VacuumGrenade::VacuumGrenade(glm::vec3 position, glm::vec3 direction, int pID, int bID, int tID)
{
	type = BULLET_TYPE::VACUUM_GRENADE;
	initValues(position, direction, pID, bID, tID);
	vel = glm::vec3(35.0f);
	damage = 10;
	maxttl = ttl;
}

VacuumGrenade::~VacuumGrenade()
{}

int VacuumGrenade::update(float dt)
{
	pos += dir * vel * dt;
	dir.y -= 1.0f * dt;

	updateWorldMat();

	ttl -= dt;
	if (ttl <= 0)	//Bullet is no longer alive and should be removed
		return 1;

	return 0;
}