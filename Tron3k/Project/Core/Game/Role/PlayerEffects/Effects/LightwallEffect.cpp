#include "LightwallEffect.h"

LightwallEffect::LightwallEffect(Player* p)
{
	myPlayer = p;
	playerVel = myPlayer->getVelocity();
}

void LightwallEffect::init(int pid, int eid, glm::vec3 position)
{
	playerId = pid; effectId = eid;
	pos = position;
	expandDong = true;
}

int LightwallEffect::update(float dt)
{
	if (!expandDong)
	{
		lifeTime -= dt;
		if (lifeTime < FLT_EPSILON)
			return 1;
	}
	else
	{
		if (myPlayer->getVelocity() != playerVel)
		{
			expandDong = false;
			lifeTime = 10.0f;
		}
	}
	
	return 0;
}