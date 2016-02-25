#include "Clusterling.h"


Clusterling::Clusterling(glm::vec3 position, glm::vec3 direction, int pID, int bID, int tID)
{
	type = BULLET_TYPE::CLUSTERLING;
	initValues(position, direction, pID, bID, tID);
	float trand = float((rand() % 100)*0.01) - 0.5f;
	maxttl = ttl = 0.9f + trand;
	vel = glm::vec3(15.0f);
	damage = 10;

	dir.y = -dir.y;
	float xrand = float((rand() % 200)*0.01) - 1.0f;
	float zrand = float((rand() % 200)*0.01) - 1.0f;
	dir.x += xrand;
	dir.z += zrand;
}

Clusterling::~Clusterling()
{}

int Clusterling::update(float dt)
{
	pos += dir * vel * dt;
	dir.y -= 1.5f * dt;

	updateWorldMat();

	ttl -= dt;
	if (ttl <= 0)	//Bullet is no longer alive and should be removed
		return 1;

	return 0;
}