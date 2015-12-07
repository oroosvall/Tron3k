#ifndef BLIT_QUAD_H
#define BLIT_QUAD_H

#include "Utils\GPUMemoryLeakChecker.h"
#include <gl/GL.h>
#include <glm\glm.hpp>
#define BUFFER_OFFSET(i) ((char *)nullptr + (i))

using namespace glm;

class BlitQuad
{
private:
	GLuint vertexDataId;
	GLuint gVertexAttribute;

	struct TriangleVertex
	{
		vec3 pos; 
		vec2 uv;
	};
	void createData(TriangleVertex* vex);
public:
	BlitQuad();
	~BlitQuad();
	void release();
	void BindVertData();
	void Init(vec3 botLeft, vec3 topRight);
	void Init(vec3 topLeft, vec3 topRight, vec3 botRight, vec3 botLeft);
};

#endif