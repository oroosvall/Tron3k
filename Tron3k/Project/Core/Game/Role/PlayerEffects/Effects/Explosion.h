#ifndef EXPLOSIONEFFECT_H
#define EXPLOSIONEFFECT_H

#include "../Effect.h"

class Explosion : public Effect
{
private:
	float exploRadius = 1.0f;
	float damage;
public:
	Explosion();
	void init(int pid, int eid, glm::vec3 position);
	void setInterestingVariable(float f) { exploRadius = f; }
	float getInterestingVariable() { return exploRadius; }
	int update(float dt);

	int getDamage() { return damage; }; //WOOO DAMAGE YO
	void setDamage(int dmg) { damage = dmg; };
	float explosionRenderRad(float* percentLifeLeft);
};
#endif