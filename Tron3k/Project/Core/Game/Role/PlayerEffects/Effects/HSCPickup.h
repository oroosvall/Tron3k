#ifndef HSCPICKUP_H
#define HSCPICKUP_H

#include "../Effect.h"

class HSCPickup : public Effect
{
private:
	float rad = 1.5f;
	float maxCooldown = 30.0f;
	float cooldown = maxCooldown;
	bool resetted = true;
public:
	HSCPickup();
	void init(int pid, int eid, glm::vec3 position);
	void setInterestingVariable(float f) { rad = f; }
	float getInterestingVariable() { return rad; }
	int update(float dt);

	bool onCooldown();
	void startCooldown() { cooldown = maxCooldown; resetted = false; };
	float getCooldown() { return cooldown; };
	void setCooldown(float cd) { cooldown = cd; }
	float renderRad();
	void setTeam() { teamId = 0; };
};
#endif