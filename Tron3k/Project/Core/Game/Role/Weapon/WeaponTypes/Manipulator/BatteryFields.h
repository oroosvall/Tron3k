#ifndef BATTERYFIELDS_H
#define BATTERYFIELDS_H

#include <glm/glm.hpp>
#include "../../Weapon.h"

#include "../../../../GameDataIndex.h"

class BatteryFields : public Weapon
{
private:
	float shootingDelay = 0.0f;
public:
	BatteryFields();
	~BatteryFields();

	void init();
	int update(float deltaTime);

	bool shoot();
	bool reload();

};
#endif