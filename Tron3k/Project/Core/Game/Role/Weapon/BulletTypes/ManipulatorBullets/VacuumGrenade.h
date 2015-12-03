#ifndef VACUUMGRENADE_H
#define VACUUMGRENADE_H

#include <glm/glm.hpp>
#include "../../Bullet.h"

#include "../../../../GameDataIndex.h"

class VacuumGrenade : public Bullet
{
private:
public:
	VacuumGrenade(glm::vec3 pos, glm::vec3 dir, int pID, int bID, int tID);
	~VacuumGrenade();

	int update(float dt);

};
#endif