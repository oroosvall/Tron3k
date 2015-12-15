#include "MeleeAttack.h"


MeleeAttack::MeleeAttack(glm::vec3 position, glm::vec3 direction, int pID, int bID, int tID, int type)
{
	initValues(position, direction, pID, bID, tID);
	vel = 25.0f;
	damage = 34;
	ttl = 0.25f;
	this->type = type;
}

MeleeAttack::~MeleeAttack()
{}

int MeleeAttack::update(float dt)
{
	pos += dir * vel * dt;

	updateWorldMat();

	ttl -= dt;
	if (ttl <= 0)	//Bullet is no longer alive and should be removed
		return 1;

	return 0;
}