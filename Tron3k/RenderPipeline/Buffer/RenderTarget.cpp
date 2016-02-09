#include "RenderTarget.h"

GLuint RenderTarget::renderQuad = 0;
GLuint RenderTarget::renderVao = 0;

void RenderTarget::init(int x, int y, int nrTex, bool depth, bool use32Bit, bool usefloat)
{
	if (renderQuad == 0)
	{
		genQuad();
	}
	
	this->depth = depth;
	glGenTextures(1, &targetId);
	resize(x, y, use32Bit, usefloat);

}

void RenderTarget::releaseStatic()
{
	glDeleteBuffers(1, &renderQuad);
	glDeleteVertexArrays(1, &renderVao);

	renderQuad = 0;
	renderVao = 0;
}

RenderTarget::~RenderTarget()
{
	glDeleteTextures(1, &targetId);
}

void RenderTarget::resize(int x, int y, bool use32bit, bool usefloat)
{
	if (x == 0)
	{
		x = 1;
	}
	if (y == 0)
	{
		//y = 1;
	}

	glBindTexture(GL_TEXTURE_2D, targetId);
	if (depth)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, x, y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	else if(use32bit)
		if(usefloat)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, x, y, 0, GL_RGB, GL_FLOAT, nullptr);
		else
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, x, y, 0, GL_RGB, GL_FLOAT, nullptr);
	else
		if (usefloat)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, x, y, 0, GL_RGB, GL_FLOAT, nullptr);
		else
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, x, y, 0, GL_RGB, GL_FLOAT, nullptr);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);

}

GLuint RenderTarget::getTargetId() const
{
	return targetId;
}

void RenderTarget::bind(GLuint target)
{

}

void RenderTarget::genQuad()
{

	struct TriangleVertex
	{
		GLfloat x, y, z;
		GLfloat u, v;
	};

	TriangleVertex tri[4] =
	{
		1.0f, 1.0f, 0.0f,
		1.0f, 1.0f,

		1.0f, -1.0f, 0.0f,
		1.0f, 0.0f,

		-1.0f, 1.0f, 0.0f,
		0.0f, 1.0f,

		-1.0f, -1.0f, 0.0f,
		0.0f, 0.0f
	};

	glGenBuffers(1, &renderQuad);
	
	glBindBuffer(GL_ARRAY_BUFFER, renderQuad);
	glBufferData(GL_ARRAY_BUFFER, sizeof(tri), tri, GL_STATIC_DRAW);
	
	glGenVertexArrays(1, &renderVao);
	glBindVertexArray(renderVao);
	
	// vertex in location 0
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(TriangleVertex), (void*)0);
	
	// uv in location 1
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(TriangleVertex), (void*)(sizeof(GLfloat)* 3));

}

void RenderTarget::render()
{

}