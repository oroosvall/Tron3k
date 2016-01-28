#ifndef EXPLOSIONEFFECT_H
#define EXPLOSIONEFFECT_H

#include "../Effect.h"
#include "../../../Player.h"

class Explosion : public Effect
{
private:
	float lifeTime;
	float exploRadius;
public:
	Explosion();
	void init(int pid, int eid, glm::vec3 position);
	void setInterestingVariable(float f) { exploRadius = f; }
	float getInterestingVariable() { return exploRadius; }
	int update(float dt);

	int getDamage() { return 15; }; //WOOO DAMAGE YO
};
#endif