#ifndef PULSERIFLE_H
#define PULSERIFLE_H

#include <glm/glm.hpp>
#include "../../Weapon.h"

#include "../../../../GameDataIndex.h"

class PulseRifle : public Weapon
{
private:
public:
	PulseRifle();
	~PulseRifle();

	void init();
	int update(float deltaTime);

	void setAmmo(int newAmmo);
};
#endif