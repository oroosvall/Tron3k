#include "BlitQuad.h"

BlitQuad::BlitQuad()
{

}

void BlitQuad::Init(GLuint* shaderProgram, vec2 bl, vec2 tr)
{
	gShaderProgram = shaderProgram;

	TriangleVertex* vex = new TriangleVertex[6];
	vex[0] = { vec3(bl.x, tr.y, 0), vec2(0, 1) };
	vex[1] = { vec3(bl.x, bl.y, 0), vec2(0, 0) };
	vex[2] = { vec3(tr.x, tr.y, 0), vec2(1, 1) };
	vex[3] = { vec3(tr.x, bl.y, 0), vec2(1, 0) };

	glGenBuffers(1, &vertexDataId);
	glBindBuffer(GL_ARRAY_BUFFER, vertexDataId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vex[0]) * 6, &vex[0], GL_STATIC_DRAW);
	delete[] vex;

	//define vertex data layout
	glGenVertexArrays(1, &gVertexAttribute);
	glBindVertexArray(gVertexAttribute);
	glEnableVertexAttribArray(0); //the vertex attribute object will remember its enabled attributes
	glEnableVertexAttribArray(1);
	//pos
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(TriangleVertex), BUFFER_OFFSET(0));
	//uv
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(TriangleVertex), BUFFER_OFFSET(sizeof(float) * 3));
#ifdef _DEBUG
	{GLenum err = glGetError(); if (err)
		int x = 0; }
#endif
}

void BlitQuad::BindVertData()
{
	glBindBuffer(GL_ARRAY_BUFFER, vertexDataId);
	glBindVertexArray(gVertexAttribute);
}