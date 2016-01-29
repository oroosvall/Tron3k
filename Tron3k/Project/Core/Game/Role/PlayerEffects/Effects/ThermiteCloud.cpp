#include "ThermiteCloud.h"

ThermiteCloud::ThermiteCloud() {}

void ThermiteCloud::init(int pid, int eid, glm::vec3 position)
{
	type = EFFECT_TYPE::THERMITE_CLOUD;
	exploRadius = 10.0f;
	playerId = pid; effectId = eid; pos = position;
	lifeTime = 5.0f;
	damageTimer = 1.0f;
}

int ThermiteCloud::update(float dt)
{
	lifeTime -= dt;
	damageTimer -= dt;
	if (lifeTime <= 0)
	{
		return 1;
	}
	if (damageTimer <= 0)
	{
		playersHitByMe.clear();
	}

	return 0;
}

float ThermiteCloud::explotionRenderRad()
{
	return  10 * exploRadius * (1 - (lifeTime / 0.1f));
}