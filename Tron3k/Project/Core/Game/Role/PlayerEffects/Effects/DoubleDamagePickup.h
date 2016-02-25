#ifndef DOUBLEDAMAGEPICKUP_H
#define DOUBLEDAMAGEPICKUP_H

#include "../Effect.h"
#include "../../../Player.h"

class DoubleDamagePickup : public Effect
{
private:
	float rad = 1.5f;
	float maxCooldown = 90.0f;
	float cooldown = maxCooldown;
	bool resetted = false;
public:
	DoubleDamagePickup();
	void init(int pid, int eid, glm::vec3 position);
	void setInterestingVariable(float f) { rad = f; }
	float getInterestingVariable() { return rad; }
	int update(float dt);

	bool onCooldown();
	void startCooldown() { cooldown = maxCooldown; resetted = false; };
	float getCooldown() { return cooldown; };
	void setCooldown(float cd) { cooldown = cd; };
	float renderRad();
	void setTeam() { teamId = 0; };
};
#endif