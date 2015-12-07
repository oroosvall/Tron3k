#ifndef LIGHTWALLEFFECT_H
#define LIGHTWALLEFFECT_H

#include "../Effect.h"
#include "../../../Player.h"

class LightwallEffect : public Effect
{
private:
	Player* myPlayer;
	float lifeTime;
	bool expandDong;
	glm::vec3 playerVel;
	glm::vec3 endPoint;
public:
	LightwallEffect(Player* p);
	void init(int pid, int eid, glm::vec3 position);
	int update(float dt);
};
#endif