#ifndef ANIMATED_TEXTURE_H
#define ANIMATED_TEXTURE_H

#include "Utils\GPUMemoryLeakChecker.h"
#include <gl/GL.h>
#include <glm\glm.hpp>
#include <vector>

using namespace glm;

struct AnimatedObject
{
	glm::mat4 worldMat;
	int roomID;
	int textureID;
	int type;

	vec2 offsets;
	float timer;
	float maxtimer;

	float segments;
};

class AnimatedTexture
{
public:
	AnimatedObject test;

	AnimatedTexture();
	~AnimatedTexture();
	void Release();

	void Initialize();
	void Update(float);
	void render();

	GLuint animQuadShader;
	GLuint animQuadUVset;
	GLuint animQuadWorld;
	GLuint animQuadVP;

private:
	std::vector<AnimatedObject> objects;
};

#endif