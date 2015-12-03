#ifndef PULSE_SHOT_H
#define PULSE_SHOT_H

#include <glm/glm.hpp>
#include "../../Bullet.h"

#include "../../../../GameDataIndex.h"

class PulseShot : public Bullet
{
private:
public:
	PulseShot(glm::vec3 pos, glm::vec3 dir, int pID, int bID, int tID);
	~PulseShot();

	int update(float dt);

};
#endif