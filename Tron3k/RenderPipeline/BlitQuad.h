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
	GLuint* gShaderProgram;

	struct TriangleVertex
	{
		vec3 pos; 
		vec2 uv;
	};
public:
	BlitQuad();
	void BindVertData();
	void Init(GLuint* shaderProgram, vec2 botLeft, vec2 topRight);
};

#endif