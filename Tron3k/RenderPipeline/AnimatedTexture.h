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
	int meshID;
	int type;

	float offset;
	float timer;
	float maxtimer;
	float segments;
};

class AnimatedTexture
{
public:
	AnimatedTexture();
	~AnimatedTexture();
	void Release();

	void Init();
	void Update(float);
	void Render();

	GLuint m_shader;
	GLuint shaderOffset;

private:
	std::vector<AnimatedObject> objects;
};

#endif