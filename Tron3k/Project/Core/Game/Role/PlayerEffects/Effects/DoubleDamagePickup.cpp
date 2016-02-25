#include "DoubleDamagePickup.h"

DoubleDamagePickup::DoubleDamagePickup() {}

void DoubleDamagePickup::init(int pid, int eid, glm::vec3 position)
{
	type = EFFECT_TYPE::DOUBLEDAMAGEPICKUP;
	lifeTime = 999.0f;
	playerId = 254; effectId = eid; pos = position;
}

int DoubleDamagePickup::update(float dt)
{
	if (cooldown > -FLT_EPSILON)
		cooldown -= dt;
	if (!resetted)
	{
		if (cooldown < FLT_EPSILON)
		{	
			if (GetSound())
			{
				GetSound()->playExternalSound(SOUNDS::announcerDoubleDamageSpawned, pos.x, pos.y, pos.z, CATEGORY::Announcer);
			}
			resetted = true;
			playersHitByMe.clear();
		}
	}

	return 0;
}

float DoubleDamagePickup::renderRad()
{
	return rad*0.5f;
}

bool DoubleDamagePickup::onCooldown()
{
	bool tester = false;
	if (cooldown > FLT_EPSILON)
	{
		tester = true;
	}
	return tester;
}