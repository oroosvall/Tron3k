#ifndef DISCHSHOT_H
#define DISCSHOT_H

#include <glm/glm.hpp>
#include "../../Bullet.h"

#include "../../../../GameDataIndex.h"

class DiscShot : public Bullet
{
private:
public:
	DiscShot(glm::vec3 pos, glm::vec3 dir, int pID, int bID, int tID);
	~DiscShot();

	int update(float dt);

};
#endif