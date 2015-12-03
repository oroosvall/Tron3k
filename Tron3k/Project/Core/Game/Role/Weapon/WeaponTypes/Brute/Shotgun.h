#ifndef SHOTGUN_H
#define SHOTGUN_H

#include <glm/glm.hpp>
#include "../../Weapon.h"

#include "../../../../GameDataIndex.h"

class Shotgun : public Weapon
{
private:
public:
	Shotgun();
	~Shotgun();

	void init();
	int update(float deltaTime);

};
#endif