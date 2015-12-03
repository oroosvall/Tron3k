#ifndef LINKGUN_H
#define LINKGUN_H

#include <glm/glm.hpp>
#include "../../Weapon.h"

#include "../../../../GameDataIndex.h"

class LinkGun : public Weapon
{
private:
public:
	LinkGun();
	~LinkGun();

	void init();
	int update(float deltaTime);

};
#endif