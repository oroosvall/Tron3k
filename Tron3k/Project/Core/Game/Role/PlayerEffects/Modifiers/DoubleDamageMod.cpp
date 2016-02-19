#include "DoubleDamageMod.h"
#include "../../../Player.h"

DoubleDamageMod::~DoubleDamageMod()
{}

void DoubleDamageMod::init(Player* myTarget)
{
	type = MODIFIER_TYPE::DOUBLEDAMAGEMOD;
	target = myTarget;
	lifeTime = 15.0f;
}

int DoubleDamageMod::getData(float dt)
{
	return 0;
}

int DoubleDamageMod::setData(float dt)
{
	lifeTime -= dt;

	if (lifeTime < FLT_EPSILON)
	{
		return 1;
	}
	return 0;
}