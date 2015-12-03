#ifndef PLASMAAUTORIFLE_H
#define PLASMAAUTORIFE_H

#include <glm/glm.hpp>
#include "../../Weapon.h"

#include "../../../../GameDataIndex.h"

class PlasmaAutorifle : public Weapon
{
private:
public:
	PlasmaAutorifle();
	~PlasmaAutorifle();

	void init();
	int update(float deltaTime);

};
#endif