#ifndef LIGHTS_H
#define LIGHTS_H

#include <gl/glew.h>
#include <gl/GL.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

struct SpotLight
{
	glm::vec3 Color;
	float DiffuseIntensity;
	glm::vec3 Position;
	float AmbientIntensity;
	glm::vec3 Direction;
	float Cutoff;

	float Constant;
	float Linear;
	float Exp;
	float padd;

	SpotLight()
	{
		Color = glm::vec3(0.0f, 0.0f, 0.0f); // 0 1 2
		Position = glm::vec3(0.0f, 0.0f, 0.0f); // 3 4 5
		Direction = glm::vec3(0.0f, -1.0f, 0.0f); // 6 7 8

		AmbientIntensity = 1.0f; // 9
		DiffuseIntensity = 0.0f; // 10
		Cutoff = 0.9f; //14

		Constant = 1.0f; //11
		Linear = 0.0f; //12
		Exp = 0.0f; //13
	}
};




#endif