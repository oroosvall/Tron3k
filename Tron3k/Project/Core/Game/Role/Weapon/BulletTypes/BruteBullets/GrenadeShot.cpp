#include "GrenadeShot.h"


GrenadeShot::GrenadeShot(glm::vec3 position, glm::vec3 direction, int pID, int bID, int tID)
{
	type = BULLET_TYPE::GRENADE_SHOT;
	initValues(position, direction, pID, bID, tID);
	vel = glm::vec3(35.0f);
	damage = 15.0f;
	maxttl = ttl = 2.0f;
}

GrenadeShot::~GrenadeShot()
{}

int GrenadeShot::update(float dt)
{
	if (ttl < 1.7f && bounceCounter < 1)
		bounceCounter = 1;
	if (ttl < 1.4f && bounceCounter < 2)
		bounceCounter = 2;
	if (ttl < 1.1f && bounceCounter < 3)
		bounceCounter = 3;
	pos += dir * vel * dt;
	dir.y -= 0.9f * dt;
	updateWorldMat();

	ttl -= dt;
	if (ttl <= 0)	//Bullet is no longer alive and should be removed
		return 1;

	return 0;
}

