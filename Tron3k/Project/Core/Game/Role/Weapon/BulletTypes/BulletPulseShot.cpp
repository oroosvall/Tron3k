#include "BulletPulseShot.h"


PulseShot::PulseShot(glm::vec3 position, glm::vec3 direction, int pID, int bID, int tID)
{

	pos = position;
	dir = direction;
	teamId = tID;
	playerId = pID;
	bulletId = bID;

	vel = 50.0f;
	damage = 10;

	//sets pos
	updateWorldMat();
	//set scale
	worldMat[0].x = 0.2f;
	worldMat[1].y = 0.2f;
	worldMat[2].z = 0.2f;

	float angleY = atan2(direction.x, direction.z) - atan2(0, 0);
	float angleX = acos(direction.y);
	rotate(angleX, -angleY, 0);
}

PulseShot::~PulseShot()
{}

int PulseShot::update(float dt)
{
	pos += dir * vel * dt;

	updateWorldMat();

	return 0;
}