#include "CleanseNova.h"

CleanseNova::CleanseNova(){}

void CleanseNova::init(int pid, int eid, glm::vec3 position)
{
	type = EFFECT_TYPE::CLEANSENOVA;
	playerId = pid; effectId = eid; pos = position;
	lifeTime = 0.2f;
}

int CleanseNova::update(float dt)
{
	lifeTime -= dt;
	if (lifeTime < FLT_EPSILON)
	{
		return 1;
	}
	return 0;
}

float CleanseNova::renderRad()
{
	return 10 * rad *(1 - (lifeTime / 0.1f));
}