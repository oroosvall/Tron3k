#ifndef DISCGUN_H
#define DISCGUN_H

#include <glm/glm.hpp>
#include "../../Weapon.h"

#include "../../../../GameDataIndex.h"

class DiscGun : public Weapon
{
private:
public:
	DiscGun();
	~DiscGun();

	void init();
	int update(float deltaTime);

};
#endif