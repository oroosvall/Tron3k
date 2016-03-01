#ifndef BATTERYSPEED_H
#define BATTERYSPEED_H

#include "../Modifier.h"

class BatterySpeedMod : public Modifier
{
private:
	float speedFactor = 1.6f;
	glm::vec3 oldDir;
	glm::vec3 vel;
	glm::vec3 airSpeed;
	float originalSens;
public:
	~BatterySpeedMod();
	void init(Player* myTarget);
	int getData(float dt);
	int setData(float dt);
	void refresh() { lifeTime = 0.5f; };
};
#endif