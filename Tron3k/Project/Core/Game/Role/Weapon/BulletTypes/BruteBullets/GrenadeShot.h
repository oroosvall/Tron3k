#ifndef GRENADESHOT_H
#define GRENADESHOT_H

#include <glm/glm.hpp>
#include "../../Bullet.h"

#include "../../../../GameDataIndex.h"

class GrenadeShot : public Bullet
{
private:
	float radius;
public:
	GrenadeShot(glm::vec3 pos, glm::vec3 dir, int pID, int bID, int tID);
	~GrenadeShot();

	int update(float dt);
	virtual float getInterestingVariable() { return radius; };
	virtual void setInterestingVariable(float f) { radius = f; };

};
#endif