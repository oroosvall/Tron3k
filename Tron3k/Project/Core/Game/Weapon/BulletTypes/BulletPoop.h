#ifndef POOP_H
#define POOP_H

#include <glm/glm.hpp>
#include "../Bullet.h"

#include "../../GameDataIndex.h"

class Poop : public Bullet
{
private:
	float vel;
public:
	Poop(glm::vec3 pos, glm::vec3 dir, int pID, int bID, int tID);
	~Poop();

	int update(float dt);

};
#endif