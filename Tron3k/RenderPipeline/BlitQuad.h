#ifndef BLIT_QUAD_H
#define BLIT_QUAD_H

#include <gl/glew.h>
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
public:
	BlitQuad();
	void BindVertData();
	void Init(vec3 botLeft, vec3 topRight);
};

#endif