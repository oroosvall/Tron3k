#pragma once
#ifndef VERTEXBUFFERUI_H
#define VERTEXBUFFERUI_H

#include <gl/glew.h>
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
			for (int i = 0; i < count; i++)
				glDeleteBuffers(1, &gVertexBuffer[i]);
			delete[] gVertexBuffer;
		}
		if (gVertexAttribute != nullptr)
		{
			for (int i = 0; i < count; i++)
				glDeleteVertexArrays(1, &gVertexAttribute[i]);
			delete[] gVertexAttribute;
		}
		if (textureIDs != nullptr)
			delete[] textureIDs;
	}
};


#endif