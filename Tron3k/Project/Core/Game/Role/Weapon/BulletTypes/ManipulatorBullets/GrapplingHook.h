#ifndef GRAPPLINGHOOK_H
#define GRAPPLINGHOOK_H

#include <glm/glm.hpp>
#include "../../Bullet.h"

#include "../../../../GameDataIndex.h"

class GrapplingHook : public Bullet
{
private:
public:
	GrapplingHook(glm::vec3 pos, glm::vec3 dir, int pID, int bID, int tID);
	~GrapplingHook();

	int update(float dt);
};
#endif