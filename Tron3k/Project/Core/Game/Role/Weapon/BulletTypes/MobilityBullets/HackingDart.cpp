#include "HackingDart.h"


HackingDart::HackingDart(glm::vec3 position, glm::vec3 direction, int pID, int bID, int tID)
{
	initValues(position, direction, pID, bID, tID);
	vel = glm::vec3(100.0f);
	damage = 20;
	type = BULLET_TYPE::HACKING_DART;
	maxttl = ttl;
}

HackingDart::~HackingDart()
{}

int HackingDart::update(float dt)
{
	pos += dir * vel * dt;

	updateWorldMat();

	ttl -= dt;
	if (ttl <= 0)	//Bullet is no longer alive and should be removed
		return 1;

	return 0;
}