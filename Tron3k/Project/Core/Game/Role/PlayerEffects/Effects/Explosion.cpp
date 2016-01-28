#include "Explosion.h"

Explosion::Explosion(){}

void Explosion::init(int pid, int eid, glm::vec3 position)
{
	type = EFFECT_TYPE::EXPLOSION;
	exploRadius = 10.0f;
	playerId = pid; effectId = eid; pos = position;
	lifeTime = 0.1f;
}

int Explosion::update(float dt)
{
	lifeTime -= dt;
	if (lifeTime < FLT_EPSILON)
	{
		return 1;
	}
	return 0;
}

float Explosion::explotionRenderRad()
{
	return  5 * exploRadius * (1 - (lifeTime / 0.1f));
}