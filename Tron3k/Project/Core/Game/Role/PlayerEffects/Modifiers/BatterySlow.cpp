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
	target->getPlayerCam()->setSensitivity(originalSens*0.5f);
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
		airSpeed.x = 0.0f; airSpeed.y = 0.0f; airSpeed.z = 0.0f;
	}
	else
	{
		if (glm::length(airSpeed) < FLT_EPSILON)
		{
			airSpeed = vel*slowFactor;
		}
		airSpeed.y -= 6.0f*dt;  //This is to simulate gravity
		target->setVelocity(airSpeed);
	}
	lifeTime -= dt;
	target->getPlayerCam()->setSensitivity(originalSens);

	if (lifeTime < FLT_EPSILON)
	{
		return 1;
	}
	return 0;
}