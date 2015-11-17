#include"Bullet.h"

Bullet::Bullet()
{
	direction = glm::vec3(0.0f, 0.0f, 0.0f);
	velocity = 0.0f;
}

Bullet::Bullet(glm::vec3 direction, float velocity)
{
	this->direction = direction;
	this->velocity = velocity;
}

Bullet::~Bullet()
{}