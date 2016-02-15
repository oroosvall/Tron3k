#ifndef LIGHTSPEED_H
#define LIGHTSPEED_H

#include "../Modifier.h"

class LightSpeed : public Modifier
{
private:
	float lightSpeed = 1.5f;
	float timer = 3.0f;
public:
	void init(Player* myTarget);
	int getData(float dt);
	int setData(float dt);
};
#endif