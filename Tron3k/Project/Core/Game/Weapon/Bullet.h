#ifndef BULLET_H
#define BULLET_H

#include <glm/glm.hpp>
#include "../GameObject.h"

class Bullet : public GameObject
{
	public:
		glm::vec3 pos;
		glm::vec3 direction;
		float velocity;
		int teamId;

		Bullet();
		Bullet(glm::vec3 pos, glm::vec3 direction, float velocity, int teamID);
		~Bullet();

		int update(float dt);

};
#endif