#include"Bullet.h"

Bullet::~Bullet()
{
	if (part)
		part->dead = true;

	if (part2)
		part2->dead = true;
}

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
	//worldMat[0].x = 0.2f;
	//worldMat[1].y = 0.2f;
	//worldMat[2].z = 0.2f;

	double angleY = atan2(direction.x, direction.z) - atan2(0, 0);
	double angleX = acos(direction.y);
	rotate(float(angleX), float(-angleY), 0);
}

void Bullet::setVel(glm::vec3 inVel)
{
	if (vel.x < 0.5)
	{
		vel.x = 0.0f;
		vel.z = 0.0f;
		vel.y = 0.0f;
	}
	else if (vel.y < 0.5)
	{
		vel.y = 0.0f;
	}
	else if (vel.z < 0.5)
	{
		vel.x = 0.0f;
		vel.z = 0.0f;
		vel.y = 0.0f;
	}
	else
		vel = inVel;
}

void Bullet::setDir(glm::vec3 newdir)
{
	resetRotation();
	double angleY = atan2(newdir.x, newdir.z) - atan2(0,0);
	double angleX = acos(newdir.y);
	rotate(float(angleX), float(-angleY), 0);

	dir = newdir;
}