#include"Bullet.h"

Bullet::Bullet()
{
	direction = glm::vec3(0.0f, 0.0f, 0.0f);
	velocity = 0.0f;
}

Bullet::Bullet(glm::vec3 position, glm::vec3 direction, float velocity, int teamId)
{

	pos = position;
	this->direction = direction;
	this->velocity = velocity;
	this->teamId = teamId;

	//sets pos
	worldMat[0].w = pos.x;
	worldMat[1].w = pos.y;
	worldMat[2].w = pos.z;
	//set scale
	worldMat[0].x = 0.03f;
	worldMat[1].y = 0.03f;
	worldMat[2].z = 0.03f;
}

Bullet::~Bullet()
{}

int Bullet::update(float dt)
{
	return 0;
}