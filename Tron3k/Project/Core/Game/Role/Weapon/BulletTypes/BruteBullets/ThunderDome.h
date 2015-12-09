#ifndef THUNDERDOME_H
#define THUNDERDOME_H

#include <glm/glm.hpp>
#include "../../Bullet.h"

#include "../../../../GameDataIndex.h"

class ThunderDome : public Bullet
{
private:
public:
	ThunderDome(glm::vec3 pos, glm::vec3 dir, int pID, int bID, int tID);
	~ThunderDome();

	int update(float dt);

};
#endif