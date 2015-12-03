#include "Modifier.h"
#include "../../Player.h"

bool Modifier::updateLifeTime(float dt)
{
	if (lifeTime > 0.0f)
	{
		lifeTimer -= dt;
		if (lifeTimer < FLT_EPSILON)
			return true;
	}
	return false;
}