#include "BatterySlowShot.h"


BatterySlowShot::BatterySlowShot(glm::vec3 position, glm::vec3 direction, int pID, int bID, int tID)
{
	type = BULLET_TYPE::BATTERY_SLOW_SHOT;
	initValues(position, direction, pID, bID, tID);
	vel = glm::vec3(50.0f);
	damage = 0;
	maxttl = ttl;
}

BatterySlowShot::~BatterySlowShot()
{}

int BatterySlowShot::update(float dt)
{
	pos += dir * vel * dt;

	updateWorldMat();

	ttl -= dt;
	if (ttl <= 0)	//Bullet is no longer alive and should be removed
		return 1;

	return 0;
}