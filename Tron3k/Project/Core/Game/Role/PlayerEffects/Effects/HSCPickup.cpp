#include "HSCPickup.h"

HSCPickup::HSCPickup() {}

void HSCPickup::init(int pid, int eid, glm::vec3 position)
{
	type = EFFECT_TYPE::HSCPICKUP;
	playerId = 254; effectId = eid; pos = position;
}

int HSCPickup::update(float dt)
{
	cooldown -= dt;
	return 0;
}

float HSCPickup::renderRad()
{
	return rad;
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