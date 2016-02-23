#include "Vacuum.h"

Vacuum::Vacuum(){}

void Vacuum::init(int pid, int eid, glm::vec3 position)
{
	type = EFFECT_TYPE::VACUUM;
	playerId = pid; effectId = eid; pos = position;
	lifeTime = 0.2f;
}

int Vacuum::update(float dt)
{
	lifeTime -= dt;
	if (lifeTime < FLT_EPSILON)
	{
		return 1;
	}
	return 0;
}

float Vacuum::renderRad()
{
	return rad * (lifeTime / 0.2f);
}

float Vacuum::lifepercentageleft()
{
	return lifeTime / 0.2f;
}