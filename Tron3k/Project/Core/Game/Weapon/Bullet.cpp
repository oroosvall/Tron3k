#include"Bullet.h"

Bullet::Bullet()
{
	direction = glm::vec3(0.0f, 0.0f, 0.0f);
	velocity = 0.0f;
}

Bullet::Bullet(glm::vec3 position, glm::vec3 direction, float velocity)
{
	pos = position;
	this->direction = direction;
	this->velocity = velocity;
}

Bullet::~Bullet()
{}

int Bullet::update(float dt)
{
	return 0;
}