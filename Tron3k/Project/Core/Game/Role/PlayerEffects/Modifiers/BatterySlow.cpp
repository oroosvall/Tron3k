#include "BatterySlow.h"
#include "../../../Player.h"

BatterySlowMod::~BatterySlowMod()
{
	target->getPlayerCam()->setSensitivity(originalSens);
}

void BatterySlowMod::init(Player* myTarget)
{
	type = MODIFIER_TYPE::BATTERYSLOWMOD;
	target = myTarget;
	lifeTime = 0.5f;
	originalSens = target->getPlayerCam()->getSensitivity();
}

int BatterySlowMod::getData(float dt)
{
	target->getPlayerCam()->setSensitivity(0.03f);
	return 0;
}

int BatterySlowMod::setData(float dt)
{
	vel = target->getVelocity();
	if (target->getGrounded())
	{
		vel.x *= slowFactor;
		vel.z *= slowFactor;
		target->setVelocity(vel);
	}
	target->setVelocity(vel);
	lifeTime -= dt;
	target->getPlayerCam()->setSensitivity(originalSens);

	if (lifeTime < FLT_EPSILON)
	{
		return 1;
	}
	return 0;
}