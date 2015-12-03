#ifndef MODIFIER_H
#define MODIFIER_H

#include <glm\glm.hpp>

class Player;

class Modifier
{
protected:
	Player* target;
	float lifeTime;
	float lifeTimer;

	bool updateLifeTime(float dt);
public:
	virtual void init(Player* myTarget) = 0;
	virtual int getData(float dt) = 0;
	virtual int setData(float dt) = 0;
};
#endif