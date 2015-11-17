#ifndef BULLET_H
#define BULLET_H

#include <glm/glm.hpp>

class Bullet
{
	private:
		glm::vec3 direction;
		float velocity;
	public:
		Bullet();
		Bullet(glm::vec3 direction, float velocity);
		~Bullet();

};
#endif