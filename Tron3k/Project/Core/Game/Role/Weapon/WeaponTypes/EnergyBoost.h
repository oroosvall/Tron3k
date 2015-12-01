#ifndef POOPGUN_H
#define POOPGUN_H

#include <glm/glm.hpp>
#include "../Weapon.h"

#include "../../../GameDataIndex.h"

class EnergyBoost : public Weapon
{
private:
public:
	EnergyBoost();
	~EnergyBoost();
	void init();
	int update(float deltaTime);
};
#endif