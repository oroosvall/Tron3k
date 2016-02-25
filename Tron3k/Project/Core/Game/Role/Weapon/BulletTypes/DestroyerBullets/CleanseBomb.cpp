#include "CleanseBomb.h"


CleanseBomb::CleanseBomb(glm::vec3 position, glm::vec3 direction, int pID, int bID, int tID)
{
	initValues(position, direction, pID, bID, tID);
	vel = glm::vec3(30.0f);
	damage = 10;
	maxttl = ttl;
}

CleanseBomb::~CleanseBomb()
{}

int CleanseBomb::update(float dt)
{
	pos += dir * vel * dt;
	dir.y -= 1.5f * dt;

	updateWorldMat();

	ttl -= dt;
	if (ttl <= 0)	//Bullet is no longer alive and should be removed
		return 1;

	return 0;
}