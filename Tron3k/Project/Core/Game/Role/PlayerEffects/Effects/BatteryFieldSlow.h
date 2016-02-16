#ifndef BATTERYFIELD_SLOW
#define BATTERYFIELD_SLOW

#include "../Effect.h"

class BatteryFieldSlow : public Effect
{
private:
	float rad = 4.0f;
	float debuffTimer;
public:
	BatteryFieldSlow();
	void init(int pid, int eid, glm::vec3 position);
	int update(float dt);

	float renderRad() { return 40.0f; };
};

#endif