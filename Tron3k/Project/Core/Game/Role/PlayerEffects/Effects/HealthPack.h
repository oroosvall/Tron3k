#ifndef HEALTHPACK_H
#define HEALTHPACK_H

#include "../Effect.h"
#include "../../../Player.h"

class HealthPack : public Effect
{
private:
	float damage;
	float size = 0.45f;
public:
	HealthPack();
	void init(int pid, int eid, glm::vec3 position);
	float getInterestingVariable() { return size; };
	int update(float dt);
};
#endif