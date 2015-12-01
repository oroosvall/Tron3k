#ifndef MODIFIER_H
#define MODIFIER_H

#include "../../Player.h"

class Modifier
{
protected:
	float lifeTime;
	float lifeTimer;

	bool updateLifeTime(float dt);
public:
	virtual void init() = 0;
	//virtual int update(Player* myTarget, float dt) = 0;
};
#endif