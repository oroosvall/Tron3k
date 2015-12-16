#ifndef OVERCHARGE_H
#define OVERCHARGE_H

#include "../Modifier.h"

class Overcharge : public Modifier
{
private:
	int startHealth;
	float timer = 8;
public:
	void init(Player* myTarget);
	int getData(float dt);
	int setData(float dt);
};
#endif