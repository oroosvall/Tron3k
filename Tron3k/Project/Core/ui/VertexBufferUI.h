#pragma once
#ifndef VERTEXBUFFERUI_H
#define VERTEXBUFFERUI_H

#include <../glew-1.11.0/include/GL/glew.h>
#include <vector>

struct VertexBuffers
{
	int count;
	GLuint* gVertexBuffer;
	GLuint* gVertexAttribute;
	GLuint* textureIDs;

	VertexBuffers()
	{
		gVertexBuffer = nullptr;
		gVertexAttribute = nullptr;
		textureIDs = nullptr;
		count = 0;
	}

	VertexBuffers( int nCount )
	{
		count = nCount;
		gVertexBuffer = new GLuint[nCount];
		gVertexAttribute = new GLuint[nCount];
		textureIDs = new GLuint[nCount];
	}

	void VertexBuffers::clean()
	{
		if (gVertexBuffer != nullptr)
		{
			glDeleteBuffers(1, gVertexBuffer);
			delete[] gVertexBuffer;
		}
		if (gVertexAttribute != nullptr)
		{
			glDeleteVertexArrays(1, gVertexAttribute);
			delete[] gVertexAttribute;
		}
		if (textureIDs != nullptr)
			delete[] textureIDs;
		count = 0;
	}
};


#endif