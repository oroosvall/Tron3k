#include "LightwallEffect.h"

LightwallEffect::LightwallEffect(Player* p)
{
	myPlayer = p;
}

void LightwallEffect::init(int pid, int eid, glm::vec3 position)
{
	playerId = pid; effectId = eid;
	pos = position;
}

int LightwallEffect::update(float dt)
{
	if (myPlayer->getRole()->getSpecialMeter() < FLT_EPSILON)
		return 1;
	return 0;
}