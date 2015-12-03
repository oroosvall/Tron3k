#ifndef SHOTGUNPELLET_H
#define SHOTGUNPELLET_H

#include <glm/glm.hpp>
#include "../../Bullet.h"

#include "../../../../GameDataIndex.h"

class ShotgunPellet : public Bullet
{
private:
public:
	ShotgunPellet(glm::vec3 pos, glm::vec3 dir, int pID, int bID, int tID);
	~ShotgunPellet();

	int update(float dt);

};
#endif