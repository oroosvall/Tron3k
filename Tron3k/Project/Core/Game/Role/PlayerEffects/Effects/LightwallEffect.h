#ifndef LIGHTWALLEFFECT_H
#define LIGHTWALLEFFECT_H

#include "../Effect.h"
#include "../../../Player.h"

class LightwallEffect : public Effect
{
private:
	Player* myPlayer;
	bool expandDong;
	bool collidable;
	glm::vec3 playerVel;
	glm::vec3 endPoint;

public:
	glm::vec3 getPlayerVel() { return playerVel; };
	void setPlayerVel(glm::vec3 inPlayerVel) { playerVel = inPlayerVel; };

	glm::vec3 getEndPoint() { return endPoint; };
	void setEndPoint(glm::vec3 inEndPoint) { endPoint = inEndPoint; };
	bool getDong() { return expandDong; }; //( ?° ?? ?°)( ?° ?? ?°) RAISE YOUR DONGERS ( ?° ?? ?°)( ?° ?? ?°)
	bool getCollidable() { return collidable; };

	LightwallEffect(Player* p);
	void init(int pid, int eid, glm::vec3 position);
	int update(float dt);
};
#endif