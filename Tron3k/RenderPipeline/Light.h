#ifndef LIGHT_H
#define LIGHT_H

#include <gl/glew.h>
#include <gl/GL.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

struct Light
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

	Light()
	{
		Color;
		DiffuseIntensity;
		Position;
		AmbientIntensity;
		Direction;
		Cutoff;

		Constant;
		Linear;
		Exp;
		padd;
	}

};

#endif