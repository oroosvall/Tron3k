#ifndef UIVERTEX_H
#define UIVERTEX_H


#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

struct uiVertex
{
	float x, y; 
	float u, v;
	
	uiVertex(){}

	uiVertex( float tx, float ty, float tu, float tv )
	{
		x = tx;
		y = ty;
		u = tu;
		v = tv;
	}

	~uiVertex(){}

	glm::vec2 getXY()
	{
		return glm::vec2(x, y);
	}
};

#endif