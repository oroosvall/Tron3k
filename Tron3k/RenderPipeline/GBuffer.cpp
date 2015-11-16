#include "GBuffer.h"

GBuffer::GBuffer()
{
	fbo = 0;
	depthTexture = 0;
	textures[GBUFFER_NUM_TEXTURES];
	for (int n = 0; n < GBUFFER_NUM_TEXTURES; n++)
	{
		textures[n] = 0;
	}
}

GBuffer::~GBuffer()
{
	if (fbo != 0) {
		glDeleteFramebuffers(1, &fbo);
	}

	if (textures[0] != 0) {
		glDeleteTextures(GBUFFER_NUM_TEXTURES, textures);
	}

	if (depthTexture != 0) {
		glDeleteTextures(1, &depthTexture);
	}
}


bool GBuffer::Init(unsigned int WindowWidth, unsigned int WindowHeight)
{
	// Create the FBO
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	// Create the gbuffer textures
	glGenTextures(GBUFFER_NUM_TEXTURES, textures);
	glGenTextures(1, &depthTexture);

	for (unsigned int i = 0; i < GBUFFER_NUM_TEXTURES; i++) {
		glBindTexture(GL_TEXTURE_2D, textures[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, WindowWidth, WindowHeight, 0, GL_RGB, GL_FLOAT, NULL);
		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, textures[i], 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	}

	//depth
	glBindTexture(GL_TEXTURE_2D, depthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, WindowWidth, WindowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	GLenum DrawBuffers[] = { GL_COLOR_ATTACHMENT0,
		GL_COLOR_ATTACHMENT1,
		GL_COLOR_ATTACHMENT2,
		GL_COLOR_ATTACHMENT3 };

	glDrawBuffers(GBUFFER_NUM_TEXTURES, DrawBuffers);

	GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (Status != GL_FRAMEBUFFER_COMPLETE) {
		int x = 0;
	}
	// restore default FBO
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

#ifdef _DEBUG
	{GLenum err = glGetError(); if (err)
		int x = 0; }
#endif
	return true;
}

void GBuffer::BindForWriting()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);

	for (int n = 0; n < 10; n++)
	{
		glActiveTexture(GL_TEXTURE0 + n);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}


void GBuffer::BindForReading()
{
	for (int n = 0; n < 4; n++)
	{
		glActiveTexture(GL_TEXTURE0 + n);
		glBindTexture(GL_TEXTURE_2D, textures[n]);
	}
	glActiveTexture(GL_TEXTURE0 + 4);
	glBindTexture(GL_TEXTURE_2D, depthTexture);
}