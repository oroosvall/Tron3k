#ifndef MODIFIER_H
#define MODIFIER_H

#include <glm\glm.hpp>
#include "../../GameDataIndex.h"

class Player;

class Modifier
{
protected:
	Player* target;
	float lifeTime;
	float lifeTimer;

	bool updateLifeTime(float dt);
	bool sticky = false;

	MODIFIER_TYPE type;
public:
	virtual ~Modifier() {};
	virtual void init(Player* myTarget) = 0;
	virtual int getData(float dt) = 0;
	virtual int setData(float dt) = 0;

	bool isSticky() { return sticky; };

	MODIFIER_TYPE getType() { return type; };
};
#endif