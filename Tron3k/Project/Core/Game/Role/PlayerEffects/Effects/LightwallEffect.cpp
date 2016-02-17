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

		if (lifeTime < 14.5f && !collidable)
		{
			collidable = true;
		}
	}
	if(expandDong)//else
	{
		glm::vec3 curVel = myPlayer->getVelocity();
		if (dot(curVel, playerVel) < 1.0-FLT_EPSILON)
		{
			int colSize = 0;
			vec4* myCols = myPlayer->getCollisionNormalsForFrame(colSize);

			for (int i = 0; i < colSize; i++)
			{
				if (myCols[i].y < 0.6f)//collides with wall or roof
				{
					vec3 thecol = vec3(myCols[i]);
					if (dot(normalize(thecol), playerVel) < 0.3f && dot(normalize(thecol), playerVel) > -0.3f)
					{
						int x = 0;
					}
				}
			}
			if (int x = 0 > 0.9f)
			{
				//Effect shall continue as new one
				endPoint = myPlayer->getPos();
				expandDong = false;
				lifeTime = 15.0f;
				if (length(endPoint - pos) < 1.5f)
					return 2;
			}
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