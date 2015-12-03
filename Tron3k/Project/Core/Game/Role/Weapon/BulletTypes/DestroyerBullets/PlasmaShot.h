#ifndef PLASMASHOT_H
#define PLASMASHOT_H

#include <glm/glm.hpp>
#include "../../Bullet.h"

#include "../../../../GameDataIndex.h"

class PlasmaShot : public Bullet
{
private:
public:
	PlasmaShot(glm::vec3 pos, glm::vec3 dir, int pID, int bID, int tID);
	~PlasmaShot();

	int update(float dt);

};
#endif