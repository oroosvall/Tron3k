#include "BatterySpeed.h"
#include "../../../Player.h"

BatterySpeedMod::~BatterySpeedMod()
{
	target->getPlayerCam()->setSensitivity(originalSens);
}

void BatterySpeedMod::init(Player* myTarget)
{
	type = MODIFIER_TYPE::BATTERYSPEEDMOD;
	target = myTarget;
	lifeTime = 0.5f;
	originalSens = target->getPlayerCam()->getSensitivity();
}

int BatterySpeedMod::getData(float dt)
{
	target->getPlayerCam()->setSensitivity(originalSens*1.5f);
	return 0;
}

int BatterySpeedMod::setData(float dt)
{
	vel = target->getVelocity();
	if (target->getGrounded())
	{
		vel.x *= speedFactor;
		vel.z *= speedFactor;
		target->setVelocity(vel);
	}
	lifeTime -= dt;

	target->getPlayerCam()->setSensitivity(originalSens);

	if (lifeTime < FLT_EPSILON)
	{
		return 1;
	}
	return 0;
}