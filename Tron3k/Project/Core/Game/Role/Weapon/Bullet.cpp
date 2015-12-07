#include"Bullet.h"

Bullet::~Bullet()
{}

void Bullet::updateWorldMat()
{
	worldMat[0].w = pos.x;
	worldMat[1].w = pos.y;
	worldMat[2].w = pos.z;
}

void Bullet::initValues(glm::vec3 position, glm::vec3 direction, int pID, int bID, int tID)
{
	pos = position;
	dir = direction;
	teamId = tID;
	playerId = pID;
	bulletId = bID;

	//sets pos
	updateWorldMat();
	//set scale
	worldMat[0].x = 0.2f;
	worldMat[1].y = 0.2f;
	worldMat[2].z = 0.2f;

	double angleY = atan2(direction.x, direction.z) - atan2(0, 0);
	double angleX = acos(direction.y);
	rotate(float(angleX), float(-angleY), 0);
}