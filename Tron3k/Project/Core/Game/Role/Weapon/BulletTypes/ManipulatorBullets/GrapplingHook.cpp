#include "GrapplingHook.h"


GrapplingHook::GrapplingHook(glm::vec3 position, glm::vec3 direction, int pID, int bID, int tID)
{
	initValues(position, direction, pID, bID, tID);
	vel = glm::vec3(90.0f);
	ttl = 6.0f;
	damage = 0;
}

GrapplingHook::~GrapplingHook()
{}

int GrapplingHook::update(float dt)
{
	pos += dir * vel * dt;

	updateWorldMat();

	ttl -= dt;
	if (ttl <= 0)	//Bullet is no longer alive and should be removed
		return 1;

	return 0;
}