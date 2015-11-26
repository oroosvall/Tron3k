#include "BulletPoop.h"


Poop::Poop(glm::vec3 position, glm::vec3 direction, int pID, int bID, int tID)
{

	pos = position;
	dir = direction;
	teamId = tID;
	playerId = pID;
	bulletId = bID;

	vel = 15.0f;
	damage = 1;

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

Poop::~Poop()
{}

int Poop::update(float dt)
{
	pos += dir * vel * dt;
	dir.y -= 1.0f*dt;

	updateWorldMat();

	return 0;
}