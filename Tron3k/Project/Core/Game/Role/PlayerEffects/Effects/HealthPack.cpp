#include "HealthPack.h"

HealthPack::HealthPack(){}

void HealthPack::init(int pid, int eid, glm::vec3 position)
{
	type = EFFECT_TYPE::HEALTHPACK;
	playerId = -1; effectId = eid; pos = position;
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