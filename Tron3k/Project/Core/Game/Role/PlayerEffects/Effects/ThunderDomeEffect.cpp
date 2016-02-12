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
	lifeTime = 8.0f;
	startLifeTime = lifeTime;
	domeRadius = 10;
}

int ThunderDomeEffect::update(float dt)
{
	lifeTime -= dt;
	if (lifeTime < FLT_EPSILON)
		return 1;
	return 0;
}

float ThunderDomeEffect::explotionRenderRad()
{
	//it will grow at 5% of it life spawn then always render the max rad
	if (lifeTime < startLifeTime * 0.98f)
		return domeRadius;
	
	return domeRadius * (startLifeTime - lifeTime) / (startLifeTime * 0.02f);
}