#ifndef FUSIONCANNON_H
#define FUSIONCANNON_H

#include <glm/glm.hpp>
#include "../../Weapon.h"

#include "../../../../GameDataIndex.h"

class FusionCannon : public Weapon
{
private:
public:
	FusionCannon();
	~FusionCannon();

	void init();
	int update(float deltaTime);

	bool shoot();
};
#endif