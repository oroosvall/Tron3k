#ifndef ENERGYSHIELD_H
#define ENERGYSHIELD_H

#include <glm/glm.hpp>
#include "../../Weapon.h"

#include "../../../../GameDataIndex.h"

class EnergyShield : public Weapon
{
private:
public:
	EnergyShield();
	~EnergyShield();

	void init();
	int update(float deltaTime);

};
#endif