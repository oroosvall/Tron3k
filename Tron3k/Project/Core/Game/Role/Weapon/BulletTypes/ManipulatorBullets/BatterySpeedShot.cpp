#include "BatterySpeedShot.h"


BatterySpeedShot::BatterySpeedShot(glm::vec3 position, glm::vec3 direction, int pID, int bID, int tID)
{
	initValues(position, direction, pID, bID, tID);
	vel = glm::vec3(25.0f);
	damage = 10;
}

BatterySpeedShot::~BatterySpeedShot()
{}

int BatterySpeedShot::update(float dt)
{
	pos += dir * vel * dt;

	updateWorldMat();

	ttl -= dt;
	if (ttl <= 0)	//Bullet is no longer alive and should be removed
		return 1;

	return 0;
}