#ifndef LIGHTSPEED_H
#define LIGHTSPEED_H

#include "../Modifier.h"

class LightSpeed : public Modifier
{
private:
	float lightSpeed = 1.3f;
	float timer = 5.0f;
	float oldTime = timer;
	glm::vec3 speed;
public:
	void init(Player* myTarget);
	int getData(float dt);
	int setData(float dt);
};
#endif