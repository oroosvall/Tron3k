#ifndef THUNDERDOMEEFFECT_H
#define THUNDERDOMEEFFECT_H

#include "../Effect.h"
#include "../../../Player.h"

class ThunderDomeEffect : public Effect
{
private:
	Player* myPlayer;
	float startLifeTime;
	float domeRadius;

public:
	ThunderDomeEffect(Player* p);
	void init(int pid, int eid, glm::vec3 position);
	int update(float dt);

	void setInterestingVariable(float f) { domeRadius = f; }
	float getInterestingVariable() { return domeRadius; }
	float explotionRenderRad();
};
#endif