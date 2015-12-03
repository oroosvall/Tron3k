#ifndef CLUSTERLING_H
#define CLUSTERLING_H

#include <glm/glm.hpp>
#include "../../Bullet.h"

#include "../../../../GameDataIndex.h"

class Clusterling : public Bullet
{
private:
public:
	Clusterling(glm::vec3 pos, glm::vec3 dir, int pID, int bID, int tID);
	~Clusterling();

	int update(float dt);

};
#endif