#ifndef LINKSHOT_H
#define LINKSHOT_H

#include <glm/glm.hpp>
#include "../../Bullet.h"

#include "../../../../GameDataIndex.h"

class LinkShot : public Bullet
{
private:
public:
	LinkShot(glm::vec3 pos, glm::vec3 dir, int pID, int bID, int tID);
	~LinkShot();

	int update(float dt);

};
#endif