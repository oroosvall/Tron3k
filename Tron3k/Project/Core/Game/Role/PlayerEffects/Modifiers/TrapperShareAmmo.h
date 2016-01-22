#ifndef TRAPPERSHAREAMMO_H
#define TRAPPERSHAREAMMO_H

#include "../Modifier.h"

class TrapperShareAmmo : public Modifier
{
private:
	int ammoCount;
	int weaponType;
public:
	void init(Player* myTarget);
	int getData(float dt);
	int setData(float dt);
};
#endif