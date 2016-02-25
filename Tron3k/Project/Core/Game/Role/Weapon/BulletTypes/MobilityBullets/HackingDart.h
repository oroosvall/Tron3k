#ifndef HACKINGDART_H
#define HACKINGDART_H

#include <glm/glm.hpp>
#include "../../Bullet.h"

#include "../../../../GameDataIndex.h"

class HackingDart : public Bullet
{
private:
public:
	HackingDart(glm::vec3 pos, glm::vec3 dir, int pID, int bID, int tID);
	~HackingDart();

	int update(float dt);

};
#endif