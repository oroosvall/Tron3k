#ifndef VACUUMEFFECT_H
#define VACUUMEFFECT_H

#include "../Effect.h"

class Vacuum : public Effect
{
private:
	float rad = 6.0f;
public:
	Vacuum();
	void init(int pid, int eid, glm::vec3 position);
	float getInterestingVariable() { return rad; };
	int update(float dt);

	float renderRad();
};
#endif