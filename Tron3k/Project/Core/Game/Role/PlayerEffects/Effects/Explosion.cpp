#include "Explosion.h"

Explosion::Explosion(){}

void Explosion::init(int pid, int eid, glm::vec3 position)
{
	exploRadius = 40.0f;
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