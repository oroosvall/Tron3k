#ifndef MELEE_H
#define MELEE_H

#include <glm/glm.hpp>
#include "../../Weapon.h"

#include "../../../../GameDataIndex.h"

class Melee : public Weapon
{
private:
public:
	Melee();
	~Melee();

	void init();
	int update(float deltaTime);

	bool shoot();

};
#endif