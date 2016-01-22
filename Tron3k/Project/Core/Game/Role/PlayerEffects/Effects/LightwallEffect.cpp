#include "LightwallEffect.h"

LightwallEffect::LightwallEffect(Player* p)
{
	myPlayer = p;
	playerVel = normalize(myPlayer->getVelocity());
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
		glm::vec3 curVel = normalize(myPlayer->getVelocity());
		if (curVel.x != playerVel.x || curVel.z != playerVel.z ||
			endPoint.y < pos.y - FLT_EPSILON || endPoint.y > pos.y + FLT_EPSILON)
		{
			expandDong = false;
			lifeTime = 100.0f;
			if (length(endPoint - pos) < 1.5f)
				return 1;
		}
		if (expandDong)
			endPoint = myPlayer->getPos();
	}
	
	return 0;
}