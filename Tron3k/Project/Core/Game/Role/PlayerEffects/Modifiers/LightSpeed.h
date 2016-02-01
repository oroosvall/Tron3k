#ifndef LIGHTSPEED_H
#define LIGHTSPEED_H

#include "../Modifier.h"

class LightSpeed : public Modifier
{
private:
	float lightSpeed = 2.0f;
	float timer = 5.0f;
public:
	void init(Player* myTarget);
	int getData(float dt);
	int setData(float dt);
};
#endif