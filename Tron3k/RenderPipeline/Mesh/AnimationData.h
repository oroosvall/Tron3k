#ifndef ANIMATIONDATA_H
#define ANIMATIONDATA_H

#include <cstdlib>
#include <glm\glm.hpp>
#include <string>

struct AnimDataHeader
{
	uint32_t jointCount;
	uint32_t keyCount;
};

struct KeyFrame
{
	glm::mat4* jointTransform;
};

struct AnimData
{
	AnimDataHeader header;
	KeyFrame* keyFrames;

	bool initialized;

	void load(std::string fileName);
	void release();
};

#endif