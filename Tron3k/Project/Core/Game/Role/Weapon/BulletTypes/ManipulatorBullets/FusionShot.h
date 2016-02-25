#ifndef FUSIONSHOT_H
#define FUSIONSHOT_H

#include <glm/glm.hpp>
#include "../../Bullet.h"

#include "../../../../GameDataIndex.h"

class FusionShot : public Bullet
{
private:
public:
	FusionShot(glm::vec3 pos, glm::vec3 dir, int pID, int bID, int tID);
	~FusionShot();

	int update(float dt);

};
#endif