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
	
	float freezeTimer;
	float swapTimer;

	bool freezeMode;

	float segments;

	int currentSegment;

	void update(float dt)
	{
		if (freezeMode)
		{
			timer -= dt;
			if (timer < 0)
			{
				freezeMode = false;
				timer = swapTimer;
			}
		}

		if (!freezeMode)
		{
			timer -= dt;


			if (timer < 0)
			{
				freezeMode = true;
				timer = freezeTimer;
				currentSegment++;
				if (currentSegment >= segments)
					currentSegment = 0;

				offsets.x = currentSegment / segments;
				offsets.y = (currentSegment + 1) / segments;
				return;
			}

			float scroll = (1 - (timer / swapTimer)) / segments;

			offsets.x = (currentSegment / segments) + scroll;
			offsets.y = ((currentSegment + 1) / segments) + scroll;
		}
	}
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