#ifndef VACUUMEFFECT_H
#define VACUUMEFFECT_H

#include "../Effect.h"

class Vacuum : public Effect
{
private:
	float rad = 12.0f;
public:
	Vacuum();
	void init(int pid, int eid, glm::vec3 position);
	float getInterestingVariable() { return rad; };
	int update(float dt);

	float renderRad();
	float lifepercentageleft();
};
#endif