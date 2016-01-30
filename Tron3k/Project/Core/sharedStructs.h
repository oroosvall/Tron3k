#ifndef SHARED_STRUCTS_H
#define SHARED_STRUCTS_H

#include <glm\glm.hpp>

struct SpawnpointG
{
	glm::vec3 pos;
	glm::vec3 dir;
	int roomID;
};

struct Decal_RenderInfo_temp
{
	float inten;
	glm::vec3 pos;
	glm::vec3 color;
	glm::vec3 normal;
	float dummy;
	float dumm2;
};


#endif