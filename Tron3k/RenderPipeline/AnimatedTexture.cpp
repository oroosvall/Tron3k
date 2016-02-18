#include "AnimatedTexture.h"

AnimatedTexture::AnimatedTexture()
{
}

AnimatedTexture::~AnimatedTexture()
{
}

void AnimatedTexture::Init()
{

/*	AnimatedObject sign;
	sign.type = 0;
	sign.maxtimer = 1.0f;
	sign.timer = sign.maxtimer;
	sign.worldMat = mat4(1.0f);
	objects.push_back(sign);
	*/

	AnimatedObject water;
	water.type = 1;
	water.maxtimer = 1.0f;
	water.worldMat = mat4(1.0f);
	water.offset = 0.0f;
	objects.push_back(water);
}

void AnimatedTexture::Update(float dT)
{
	for (int i = 0; i < objects.size(); i++)
	{
		AnimatedObject* o = &objects[i];

		//Timer calculations
		o->timer -= dT;

		//Sign
		if (o->type == 0)
		{
			if (o->timer < 0)
			{
				//swap
				o->timer = o->maxtimer;

				//Set offset
				float movement = 1.0f / o->segments;
				o->offset += movement;
			}
		}

		//Water
		else
		{
			o->offset += 0.001f * dT;
		}
	}
}