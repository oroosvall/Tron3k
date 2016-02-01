#include "ThermiteCloud.h"

ThermiteCloud::ThermiteCloud() {}

void ThermiteCloud::init(int pid, int eid, glm::vec3 position)
{
	type = EFFECT_TYPE::THERMITE_CLOUD;
	exploRadius = 2.0f;
	playerId = pid; effectId = eid; pos = position;
	lifeTime = 5.0f;
	damageTimer = 0.5f;
	damage = 8;	
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
		damageTimer = 0.5;
	}

	return 0;
}

float ThermiteCloud::explotionRenderRad()
{
	return  20 * exploRadius;
}