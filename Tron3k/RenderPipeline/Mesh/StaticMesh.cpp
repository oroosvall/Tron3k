#include "StaticMesh.h"

void StaticMesh::release()
{

	glDeleteBuffers(1, &vertexBuffer);
	glDeleteBuffers(1, &indexBuffer);

	glDeleteVertexArrays(1, &vertexArray);

}

void StaticMesh::init(int iCount)
{
	instanceCount = iCount;
}

void StaticMesh::setVertices(float* &vertices, int vertCount)
{
	vertexCount = vertCount;
	verts = (Vertex11*)vertices;
}

void StaticMesh::setIndices(int* &inds, int indCount)
{
	indexCount = indCount;
	indices = (Index*)inds;
}

void StaticMesh::stream()
{
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts[0]) * vertexCount, verts, GL_STATIC_DRAW);

	glGenBuffers(1, &indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
#ifdef _DEBUG
	if ((sizeof(unsigned int)*indexCount) == _msize(indices))
	{
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indexCount, &indices[0], GL_STATIC_DRAW);
	}
	else
	{
		indexCount = 0;
	}
#else
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indexCount, &indices[0], GL_STATIC_DRAW);
#endif

	//define vertex data layout
	glGenVertexArrays(1, &vertexArray);
	glBindVertexArray(vertexArray);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);

#define BUFFER_OFFSET(i) ((char *)nullptr + (i))

	//pos
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex11), BUFFER_OFFSET(0));
	//uv
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex11), BUFFER_OFFSET(sizeof(float) * 3));
	// normal
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex11), BUFFER_OFFSET(sizeof(float) * 5));
	// tangent
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(sizeof(float) * 8));

	delete[] verts;
	delete[] indices;
}