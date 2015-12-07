#include "BlitQuad.h"

BlitQuad::BlitQuad()
{

}

BlitQuad::~BlitQuad()
{
}

void BlitQuad::release()
{
	glDeleteBuffers(1, &vertexDataId);
	glDeleteVertexArrays(1, &gVertexAttribute);
}

void BlitQuad::Init(vec3 bl, vec3 tr)
{
	TriangleVertex* vex = new TriangleVertex[6];
	vex[0] = { vec3(bl.x, tr.y, bl.z), vec2(0, 1) };
	vex[1] = { vec3(bl.x, bl.y, bl.z), vec2(0, 0) };
	vex[2] = { vec3(tr.x, tr.y, tr.z), vec2(1, 1) };
	vex[3] = { vec3(tr.x, bl.y, tr.z), vec2(1, 0) };

	createData(vex);
}

void BlitQuad::Init(vec3 topLeft, vec3 topRight, vec3 botRight, vec3 botLeft)
{
	TriangleVertex* vex = new TriangleVertex[6];
	vex[0] =  { topLeft, vec2(0, 1) };
	vex[1] =  { botLeft, vec2(0, 0) };
	vex[2] =  { topRight, vec2(1, 1) };
	vex[3] =  { botRight, vec2(1, 0) };

	createData(vex);
}

void BlitQuad::createData(TriangleVertex* vex)
{
	glGenBuffers(1, &vertexDataId);
	glBindBuffer(GL_ARRAY_BUFFER, vertexDataId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vex[0]) * 4, &vex[0], GL_STATIC_DRAW);
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
}

void BlitQuad::BindVertData()
{
	glBindBuffer(GL_ARRAY_BUFFER, vertexDataId);
	glBindVertexArray(gVertexAttribute);
}