#include "Explosion.h"

Explosion::Explosion(){}

void Explosion::init(int pid, int eid, glm::vec3 position)
{
	type = EFFECT_TYPE::EXPLOSION;
	playerId = pid; effectId = eid; pos = position;
	lifeTime = 0.2f;
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

float Explosion::explosionRenderRad(float* percentLifeLeft)
{
	*percentLifeLeft = lifeTime / 0.2f;
	return exploRadius * (1 - (lifeTime / 0.2f));
}