#include "SimpleShit.h"

struct TriangleVertexWateverThingymajiggy
{
	float x, y, z;
};

struct IndexThingymajiggy
{
	unsigned int f1, f2, f3;
};

void TestMesh::make()
{
	TriangleVertexWateverThingymajiggy vertex[] =
	{
		0, 0, 0,
		0, 1, 0,
		1, 0, 0
	};

	IndexThingymajiggy indexstr[] =
	{
		0, 1, 2
	};

	glGenBuffers(1, &vbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vbuffer);

	glBufferDataARB(GL_ARRAY_BUFFER, sizeof(vertex), &vertex, GL_STATIC_DRAW_ARB);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glGenBuffers(1, &index);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index);

	glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexstr), &indexstr, GL_STATIC_DRAW_ARB);

}