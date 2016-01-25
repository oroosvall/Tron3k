#pragma once
#ifndef VERTEX_T_H
#define VERTEX_T_H

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

struct Vertex_ui
{
	float x, y;
	float u, v;

	Vertex_t() {}

	Vertex_t(float x, float y, float u, float v)
	{
		this->x = x;
		this->y = y;
		this->u = u;
		this->v = v;
	}

	~Vertex_t() {}
};

#endif
