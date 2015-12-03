#ifndef BATTERYSPEEDSHOT_H
#define BATTERYSPEEDSHOT_H

#include <glm/glm.hpp>
#include "../../Bullet.h"

#include "../../../../GameDataIndex.h"

class BatterySpeedShot : public Bullet
{
private:
public:
	BatterySpeedShot(glm::vec3 pos, glm::vec3 dir, int pID, int bID, int tID);
	~BatterySpeedShot();

	int update(float dt);

};
#endif