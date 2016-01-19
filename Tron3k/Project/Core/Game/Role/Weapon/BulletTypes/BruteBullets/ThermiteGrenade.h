#ifndef THERMITEGRENADE_H
#define THERMITEGRENADE_H

#include <glm/glm.hpp>
#include "../../Bullet.h"

#include "../../../../GameDataIndex.h"

class ThermiteGrenade : public Bullet
{
private:
public:
	ThermiteGrenade(glm::vec3 pos, glm::vec3 dir, int pID, int bID, int tID);
	int update(float dt);

};
#endif