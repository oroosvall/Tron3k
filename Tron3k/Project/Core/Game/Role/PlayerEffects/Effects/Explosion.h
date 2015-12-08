#ifndef EXPLOSIONEFFECT_H
#define EXPLOSIONEFFECT_H

#include "../Effect.h"
#include "../../../Player.h"

class Explosion : public Effect
{
private:
	float lifeTime;
public:
	Explosion();
	void init(int pid, int eid, glm::vec3 position);
	int update(float dt);

	int getDamage() { return 15; }; //WOOO DAMAGE YO
};
#endif