#ifndef LIGHTWALLCONTROLLOCK_H
#define LIGHTWALLCONTROLLOCK_H

#include "../Modifier.h"

class LightWallLockedControls : public Modifier
{
private:
	float lightWallSpeed;
	glm::vec3 vel;
	glm::vec3 normvel;
	float stopCooldown;
public:
	~LightWallLockedControls();
	void init(Player* myTarget);
	int getData(float dt);
	int setData(float dt);
};
#endif