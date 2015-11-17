#include "GBuffer.h"

Gbuffer::Gbuffer()
{
	targetId = 0;
	initialized = false;
	blitQuads = nullptr;
	pos = nullptr;
}

void Gbuffer::init(int x, int y, int nrTex, bool depth)
{

	if (renderQuad == 0)
	{
		genQuad();
	}

	// gbuffer

	glGenFramebuffers(1, &targetId);
	bind(GL_FRAMEBUFFER);
	this->depth = depth;
	nrTextures = nrTex;

	generate(x, y);


	GLenum* DrawBuffers = new GLenum[nrTextures];
	
	for (int i = 0; i < nrTextures; i++)
	{
		if (i == 0 && depth)
		{
			glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, rTexture[i].getTargetId(), 0);
			DrawBuffers[i] = GL_NONE;
		}
		else
		{
			glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, rTexture[i].getTargetId(), 0);
			DrawBuffers[i] = (GL_COLOR_ATTACHMENT0 + i);
		}
	}

	glDrawBuffers(nrTextures, DrawBuffers);

	delete[] DrawBuffers;

	GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (Status != GL_FRAMEBUFFER_COMPLETE) {
		throw;
	}

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	
	if (!shaderPtr)
	{
		throw;
	}

	pos = new GLuint[nrTextures];

	if (depth)
	{
		pos[4] = glGetUniformLocation(*shaderPtr, "Depth");;
	}
	pos[0] = glGetUniformLocation(*shaderPtr, "Position");
	pos[1] = glGetUniformLocation(*shaderPtr, "Diffuse");
	pos[2] = glGetUniformLocation(*shaderPtr, "Normal");
	pos[3] = glGetUniformLocation(*shaderPtr, "UVcord");;
	pos[5] = glGetUniformLocation(*shaderPtr, "Use");

	blitQuads = new BlitQuad[6];
	blitQuads[0].Init(shaderPtr, vec2(-1, -1), vec2(-0.6, -0.6));
	blitQuads[1].Init(shaderPtr, vec2(-0.6, -1), vec2(-0.2, -0.6));
	blitQuads[2].Init(shaderPtr, vec2(-0.2, -1), vec2(0.2, -0.6));
	blitQuads[3].Init(shaderPtr, vec2(0.2, -1), vec2(0.6, -0.6));
	blitQuads[4].Init(shaderPtr, vec2(0.6, -1), vec2(1, -0.6));
	blitQuads[5].Init(shaderPtr, vec2(-1, -1), vec2(1, 1));

	initialized = true;
}

Gbuffer::~Gbuffer()
{
	delete[] rTexture;
	delete[] pos;
	delete[] blitQuads;
	if (initialized)
	{
		glDeleteFramebuffers(1, &targetId);
		//Debug::DebugOutput("Deleting gbuffer target\n");
	}
}

void Gbuffer::resize(int x, int y)
{
	for (int i = 0; i < nrTextures; i++)
	{
		rTexture[i].resize(x, y);
	}
}

void Gbuffer::bind(GLuint target)
{
	glBindFramebuffer(target, targetId);
}

void Gbuffer::render()
{
	// bind shader
	glUseProgram(*shaderPtr);

	// bind textures
	for (int i = 1; i < nrTextures; i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, rTexture[i].getTargetId());
		glProgramUniform1i(*shaderPtr, pos[i], i);
	}
	
	// bind buffer
	glBindBuffer(GL_ARRAY_BUFFER, renderQuad);
	glBindVertexArray(renderVao);

	glProgramUniform1i(*shaderPtr, pos[0], 0);
	glProgramUniform1i(*shaderPtr, pos[1], 1);
	glProgramUniform1i(*shaderPtr, pos[2], 2);
	glProgramUniform1i(*shaderPtr, pos[3], 3);
	glProgramUniform1i(*shaderPtr, pos[4], 4);

	blitQuads[5].BindVertData();
	glProgramUniform1i(*shaderPtr, pos[5], 5);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	//each blit
	for (int n = 0; n < 5; n++)
	{
		blitQuads[n].BindVertData();
		glProgramUniform1i(*shaderPtr, pos[5], n);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}
}


void Gbuffer::generate(int x, int y)
{
	rTexture = new RenderTarget[nrTextures];
	for (int i = 0; i < nrTextures; i++)
	{
		if (i == 0 && depth)
		{
			rTexture[i].init(x, y, 0, true);
		}
		else
		{
			rTexture[i].init(x, y, 0, false);
		}
		
	}
}
