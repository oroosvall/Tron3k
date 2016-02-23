#ifndef THERMITECLOUD_H
#define THERMITECLOUD_H

#include "../Effect.h"
#include "../../../Player.h"

class ThermiteCloud : public Effect
{
private:
	float startingLifetime;
	float exploRadius;
	int damage;
	float damageTimer;
public:
	ThermiteCloud();
	void init(int pid, int eid, glm::vec3 position);
	float getInterestingVariable() { return exploRadius; }
	int update(float dt);

	int getDamage() { return damage; }; //WOOO DAMAGE YO
	float explosionRenderRad();
	float lifetimepercentleft();
};
#endif