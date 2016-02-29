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
	endPoint = pos = position-playerVel;
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
	}
	if (expandDong)//else
	{
		if (!myPlayer->searchModifier(MODIFIER_TYPE::LIGHTWALLCONTROLLOCK))
		{
			endPoint = myPlayer->getPos() + playerVel;
			expandDong = false;
			collidable = true;
			lifeTime = 10.0f;
			if (length(endPoint - pos) < 4.0f)
				return 1;
		}
		if (expandDong)
			endPoint = myPlayer->getPos();
	}
	return 0;
}