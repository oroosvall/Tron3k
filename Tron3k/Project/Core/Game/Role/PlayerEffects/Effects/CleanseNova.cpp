#include "CleanseNova.h"

CleanseNova::CleanseNova(){}

void CleanseNova::init(int pid, int eid, glm::vec3 position)
{
	type = EFFECT_TYPE::CLEANSENOVA;
	playerId = pid; effectId = eid; pos = position;
	lifeTime = 0.3f;
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
		return rad * (1 - (lifeTime / 0.3f));
}

float CleanseNova::lifepercentageleft()
{
	return (lifeTime / 0.3f);
}