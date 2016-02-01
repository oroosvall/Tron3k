#ifndef THERMITECLOUD_H
#define THERMITECLOUD_H

#include "../Effect.h"
#include "../../../Player.h"

class ThermiteCloud : public Effect
{
private:
	float lifeTime;
	float startingLifetime;
	float exploRadius;
	float damage = 10;
	float damageTimer;
public:
	ThermiteCloud();
	void init(int pid, int eid, glm::vec3 position);
	void setInterestingVariable(float f) { exploRadius = f; }
	float getInterestingVariable() { return exploRadius; }
	int update(float dt);

	int getDamage() { return damage; }; //WOOO DAMAGE YO
	void setDamage(int dmg) { damage = dmg; };
	float explotionRenderRad();
};
#endif