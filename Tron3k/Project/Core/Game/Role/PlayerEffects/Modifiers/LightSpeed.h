#ifndef LIGHTSPEED_H
#define LIGHTSPEED_H

#include "../Modifier.h"

class LightSpeed : public Modifier
{
private:
	float lightSpeed = 50.0;
	float timer = 8;
public:
	~LightSpeed();
	void init(Player* myTarget);
	int getData(float dt);
	int setData(float dt);
};
#endif