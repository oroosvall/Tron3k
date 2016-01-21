#ifndef VERTEX_H
#define VERTEX_H


#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

struct Vertex
{
	float x, y; 
	float u, v;
	
	Vertex(){}

	Vertex( float tx, float ty, float tu, float tv )
	{
		x = tx;
		y = ty;
		u = tu;
		v = tv;
	}

	~Vertex(){}

	glm::vec2 getXY()
	{
		return glm::vec2(x, y);
	}
};

#endif