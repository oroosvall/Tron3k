#ifndef PARTICLESTRUCT_H
#define PARTICLESTRUCT_H

#include <glm\glm.hpp>

struct MovableParticle
{
	bool created = false;
	bool dead = false;
	bool allowRemove = false;
	int id;

	glm::vec3 color;
	glm::vec3* pPos;
};

#endif