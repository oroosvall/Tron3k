#ifndef LIGHTWALLEFFECT_H
#define LIGHTWALLEFFECT_H

#include "../Effect.h"

class LightwallEffect : public Effect
{
private:
	float lifeTime;
public:
	void init();
	int update(float dt);
};
#endif