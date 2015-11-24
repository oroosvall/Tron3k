#include "GBuffer.h"

Gbuffer::Gbuffer()
{
	targetId = 0;
	initialized = false;
	blitQuads = nullptr;
	uniformBitsList = nullptr;
	nrOfLights = 0;
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

	uniformBitsList = new GLuint[nrTextures];

	uniformEyePos = glGetUniformLocation(*shaderPtr, "eyepos");
	//uniformNrOfLight = glGetUniformLocation(*shaderPtr, "NumSpotLights");

	if (depth)
	{
		uniformBitsList[0] = glGetUniformLocation(*shaderPtr, "Depth");;
	}
	uniformBitsList[1] = glGetUniformLocation(*shaderPtr, "Position");
	uniformBitsList[2] = glGetUniformLocation(*shaderPtr, "Diffuse");
	uniformBitsList[3] = glGetUniformLocation(*shaderPtr, "Normal");
	uniformBitsList[4] = glGetUniformLocation(*shaderPtr, "UVcord");;
	uniformUse = glGetUniformLocation(*shaderPtr, "Use");


	blitQuads = new BlitQuad[6];
	blitQuads[0].Init(shaderPtr, vec2(-1, -1), vec2(-0.6, -0.6));
	blitQuads[1].Init(shaderPtr, vec2(-0.6, -1), vec2(-0.2, -0.6));
	blitQuads[2].Init(shaderPtr, vec2(-0.2, -1), vec2(0.2, -0.6));
	blitQuads[3].Init(shaderPtr, vec2(0.2, -1), vec2(0.6, -0.6));
	blitQuads[4].Init(shaderPtr, vec2(0.6, -1), vec2(1, -0.6));
	blitQuads[5].Init(shaderPtr, vec2(-1, -1), vec2(1, 1));

	initialized = true;
}

//void Gbuffer::initLight()
//{
//
//}

Gbuffer::~Gbuffer()
{
	delete[] rTexture;
	delete[] uniformBitsList;
	delete[] blitQuads;
	if (initialized)
	{
		glDeleteFramebuffers(1, &targetId);
		//Debug::DebugOutput("Deleting gbuffer target\n");
	}

	glDeleteShader(*shaderPtr);
	delete shaderPtr;
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
	for (int i = 0; i < nrTextures; i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, rTexture[i].getTargetId());
		glProgramUniform1i(*shaderPtr, uniformBitsList[i], i);
	}

	glProgramUniform3fv(*shaderPtr, uniformEyePos, 1, &eyePos[0]);
	//glProgramUniform1i(*shaderPtr, nrLightLoc, nrOfLights);

	// bind buffer
	glBindBuffer(GL_ARRAY_BUFFER, renderQuad);
	glBindVertexArray(renderVao);

	blitQuads[5].BindVertData();
	glProgramUniform1i(*shaderPtr, uniformUse, 5);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	//each blit
	for (int n = 0; n < 5; n++)
	{
		blitQuads[n].BindVertData();
		glProgramUniform1i(*shaderPtr, uniformUse, n);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}
}

void Gbuffer::clearLights()
{
	nrOfLights = 0;
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
