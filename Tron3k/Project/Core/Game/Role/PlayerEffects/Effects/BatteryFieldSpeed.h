#ifndef BATTERYFIELD_SPEED
#define BATTERYFIELD_SPEED

#include "../Effect.h"

class BatteryFieldSpeed : public Effect
{
private:
	float rad = 4.0f;
	float debuffTimer;
public:
	BatteryFieldSpeed();
	void init(int pid, int eid, glm::vec3 position);
	int update(float dt);
	float getInterestingVariable() { return rad; };
	float renderRad() { return 40.0f; };
};

#endif