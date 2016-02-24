#include "HSCPickup.h"

HSCPickup::HSCPickup() {}

void HSCPickup::init(int pid, int eid, glm::vec3 position)
{
	type = EFFECT_TYPE::HSCPICKUP;
	lifeTime = 999.0f;
	playerId = 254; effectId = eid; pos = position;
}

int HSCPickup::update(float dt)
{
	if(cooldown > -FLT_EPSILON)
		cooldown -= dt;
	if (!resetted)
	{
		if (cooldown < FLT_EPSILON)
		{
			resetted = true;
			playersHitByMe.clear();
		}
	}

	return 0;
}

float HSCPickup::renderRad()
{
	return rad*0.5f;
}

bool HSCPickup::onCooldown()
{
	bool tester = false;
	if (cooldown > FLT_EPSILON)
	{
		tester = true;
	}
	return tester;
}