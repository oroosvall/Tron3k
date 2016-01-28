#include "ThunderDomeEffect.h"

ThunderDomeEffect::ThunderDomeEffect(Player* p)
{
	myPlayer = p;
}

void ThunderDomeEffect::init(int pid, int eid, glm::vec3 position)
{
	type = EFFECT_TYPE::THUNDER_DOME;
	playerId = pid; effectId = eid;
	pos = position;
	lifeTime = 1000.0f;
	domeRadius = 10;
}

int ThunderDomeEffect::update(float dt)
{
	lifeTime -= dt;
	if (lifeTime < FLT_EPSILON)
		return 1;
	return 0;
}