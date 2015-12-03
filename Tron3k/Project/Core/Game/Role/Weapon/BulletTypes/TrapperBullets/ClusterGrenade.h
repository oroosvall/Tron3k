#ifndef CLUSTERGRENADE_H
#define CLUSTERGRENADE_H

#include <glm/glm.hpp>
#include "../../Bullet.h"

#include "../../../../GameDataIndex.h"

class ClusterGrenade : public Bullet
{
private:
public:
	ClusterGrenade(glm::vec3 pos, glm::vec3 dir, int pID, int bID, int tID);
	~ClusterGrenade();

	int update(float dt);

};
#endif