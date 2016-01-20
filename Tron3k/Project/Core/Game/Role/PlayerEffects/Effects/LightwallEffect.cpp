#include "LightwallEffect.h"

LightwallEffect::LightwallEffect(Player* p)
{
	myPlayer = p;
	playerVel = myPlayer->getVelocity();
}

void LightwallEffect::init(int pid, int eid, glm::vec3 position)
{
	type = EFFECT_TYPE::LIGHT_WALL;
	playerId = pid; effectId = eid;
	endPoint = pos = position;
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
		if (myPlayer->getVelocity().x != playerVel.x || myPlayer->getVelocity().z != playerVel.z)
		{
			expandDong = false;
			lifeTime = 10.0f;
			if (length(endPoint - pos) < 1.5f)
				return 1;
		}
		if (expandDong)
			endPoint = myPlayer->getPos();
	}
	
	return 0;
}