#ifndef VERTEX_H
#define VERTEX_H

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include "Vertex_ui.h"

struct uiVertex
{
	Vertex_ui vertexList[4];
	
	uiVertex(){}

	uiVertex( glm::vec2 pos[], glm::vec2 uv[] )
	{
		for (int i = 0; i < 4; i++)
		{
			vertexList[i] = Vertex_ui(pos[i].x, pos[i].y, uv[i].x, uv[i].y);
		}
		
	}

	~uiVertex(){}
};

#endif