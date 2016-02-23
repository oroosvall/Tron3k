#ifndef BATTERYSLOW_H
#define BATTERYSLOW_H

#include "../Modifier.h"

class BatterySlowMod : public Modifier
{
private:
	float slowFactor = 0.4f;
	glm::vec3 vel;
	glm::vec3 airSpeed;
	float originalSens;
public:
	~BatterySlowMod();
	void init(Player* myTarget);
	int getData(float dt);
	int setData(float dt);
	void refresh() { lifeTime = 0.5f; };
};
#endif