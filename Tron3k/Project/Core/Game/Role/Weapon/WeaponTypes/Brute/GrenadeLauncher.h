#ifndef GRENADELAUNCHER_H
#define GRENADELAUNCHER_H

#include <glm/glm.hpp>
#include "../../Weapon.h"

#include "../../../../GameDataIndex.h"

class GrenadeLauncher : public Weapon
{
private:
public:
	GrenadeLauncher();
	~GrenadeLauncher();

	void init();
	int update(float deltaTime);

};
#endif