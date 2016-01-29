#include "ThermiteEffect.h"

ThermiteEffect::ThermiteEffect() {}

void ThermiteEffect::init(int pid, int eid, glm::vec3 position)
{
	type = EFFECT_TYPE::EXPLOSION;
	exploRadius = 10.0f;
	playerId = pid; effectId = eid; pos = position;
	lifeTime = 5.0f;
}

int ThermiteEffect::update(float dt)
{
	lifeTime -= dt;
	if (lifeTime < FLT_EPSILON)
	{
		return 1;
	}
	return 0;
}

float ThermiteEffect::explotionRenderRad()
{
	return  10 * exploRadius * (1 - (lifeTime / 0.1f));
}