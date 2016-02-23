#include "BatteryFieldSlow.h"

BatteryFieldSlow::BatteryFieldSlow()
{

}

void BatteryFieldSlow::init(int pid, int eid, glm::vec3 position)
{
	playerId = pid;
	effectId = eid;
	pos = position;
	type = EFFECT_TYPE::BATTERY_SLOW;
	debuffTimer = 0.3f;
	lifeTime = 2.0f;
}

int BatteryFieldSlow::update(float dt)
{
	lifeTime -= dt;
	debuffTimer -= dt;
	if (lifeTime < FLT_EPSILON)
		return 1;
	if (debuffTimer <= 0)
	{
		playersHitByMe.clear();
		debuffTimer = 0.3f;
	}
	return 0;
}