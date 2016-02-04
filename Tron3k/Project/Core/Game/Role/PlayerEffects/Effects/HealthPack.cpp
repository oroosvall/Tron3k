#include "HealthPack.h"

HealthPack::HealthPack(){}

void HealthPack::init(int pid, int eid, glm::vec3 position)
{
	type = EFFECT_TYPE::HEALTHPACK;
	playerId = 254; effectId = eid; pos = position; //Player ID is the maximum allowed for Uint8. We shouldn't have this many players, and if we do: Good job healing!
	lifeTime = 4.5f;
}

int HealthPack::update(float dt)
{
	lifeTime -= dt;
	if (lifeTime < FLT_EPSILON)
	{
		return 1;
	}
	return 0;
}