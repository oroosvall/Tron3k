#ifndef HSCPICKUP_H
#define HSCPICKUP_H

#include "../Effect.h"

class HSCPickup : public Effect
{
private:
	float rad = 6.0f;
	float maxCooldown = 20.0f;
	float cooldown = maxCooldown;
public:
	HSCPickup();
	void init(int pid, int eid, glm::vec3 position);
	void setInterestingVariable(float f) { rad = f; }
	float getInterestingVariable() { return rad; }
	int update(float dt);

	bool onCooldown();
	void startCooldown() { cooldown = maxCooldown; };
	float getCooldown() { return cooldown; };
	float renderRad();
	void setTeam() { teamId = 0; };
};
#endif