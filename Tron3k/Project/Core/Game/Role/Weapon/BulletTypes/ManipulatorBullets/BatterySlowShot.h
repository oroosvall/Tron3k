#ifndef BATTERYSLOWSHOT_H
#define BATTERYSLOWSHOT_H

#include <glm/glm.hpp>
#include "../../Bullet.h"

#include "../../../../GameDataIndex.h"

class BatterySlowShot : public Bullet
{
private:
public:
	BatterySlowShot(glm::vec3 pos, glm::vec3 dir, int pID, int bID, int tID);
	~BatterySlowShot();

	int update(float dt);

};
#endif