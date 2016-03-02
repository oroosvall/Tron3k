#ifndef PARTICLESTRUCT_H
#define PARTICLESTRUCT_H

#include <glm\glm.hpp>

struct MovableParticle
{
	BULLET_TYPE bt = NROFBULLETS; //One of these values will change
	EFFECT_TYPE et = NROFEFFECTS; //The other will not. Use the right value
	bool created = false;
	bool dead = false;
	bool allowRemove = false;
	int id;

	glm::vec3 color;
	glm::vec3* pPos;
	glm::vec3 dir;
};

#endif