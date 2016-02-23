#ifndef CLEANSENOVA_H
#define CLEANSENOVA_H

#include "../Effect.h"

class CleanseNova : public Effect
{
private:
	float rad = 15.0f;
	float damage;
public:
	CleanseNova();
	void init(int pid, int eid, glm::vec3 position);
	float getInterestingVariable() { return rad; }
	int update(float dt);

	float renderRad();
	float lifepercentageleft();
};
#endif