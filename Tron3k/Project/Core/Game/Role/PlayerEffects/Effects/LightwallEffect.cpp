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
	collidable = false;
}

int LightwallEffect::update(float dt)
{
	if (!expandDong)
	{
		lifeTime -= dt;
		if (lifeTime < FLT_EPSILON)
			return 1;

		if (lifeTime < 9.5f && !collidable)
		{
			collidable = true;
		}
	}
	else
	{
		glm::vec3 curVel = normalize(myPlayer->getVelocity());
		if (dot(curVel, playerVel) < 1.0-FLT_EPSILON)
		{
			endPoint = myPlayer->getPos();
			expandDong = false;
			lifeTime = 15.0f;
			if (length(endPoint - pos) < 1.5f)
				return 1;
		}
		if (expandDong)
			endPoint = myPlayer->getPos();
	}
	
	return 0;
}