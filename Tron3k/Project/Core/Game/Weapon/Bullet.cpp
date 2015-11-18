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
	worldMat[0].x = 0.2f;
	worldMat[1].y = 0.2f;
	worldMat[2].z = 0.2f;

	float angleY = atan2(direction.x, direction.z) - atan2(0, 0);
	
	rotate(0, -angleY, 0);
}

Bullet::~Bullet()
{}

int Bullet::update(float dt)
{
	pos += direction * velocity * dt;

	worldMat[0].w = pos.x;
	worldMat[1].w = pos.y;
	worldMat[2].w = pos.z;

	return 0;
}