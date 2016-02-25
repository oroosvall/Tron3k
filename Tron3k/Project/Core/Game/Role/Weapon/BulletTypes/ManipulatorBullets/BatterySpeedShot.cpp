#include "BatterySpeedShot.h"


BatterySpeedShot::BatterySpeedShot(glm::vec3 position, glm::vec3 direction, int pID, int bID, int tID)
{
	type = BULLET_TYPE::BATTERY_SPEED_SHOT;
	initValues(position, direction, pID, bID, tID);
	vel = glm::vec3(50.0f);
	damage = 0;
	maxttl = ttl;
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