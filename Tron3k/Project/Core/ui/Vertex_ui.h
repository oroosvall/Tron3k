#pragma once
#ifndef VERTEX_UI_H
#define VERTEX_UI_H

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

struct Vertex_ui
{
	float x, y;
	float u, v;

	Vertex_ui() {}

	Vertex_ui(float x, float y, float u, float v)
	{
		this->x = x;
		this->y = y;
		this->u = u;
		this->v = v;
	}

	~Vertex_ui() {}
};

#endif
