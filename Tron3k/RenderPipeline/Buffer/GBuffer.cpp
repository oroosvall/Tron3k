#include "GBuffer.h"

Gbuffer::Gbuffer()
{
	targetId = 0;
	initialized = false;
	blitQuads = nullptr;
	uniformBitsList = nullptr;
	nrOfSpotLights = 0;
	nrOfPointLights = 0;
}

void Gbuffer::init(int x, int y, int nrTex, bool depth)
{
	xres = (float)x;
	yres = (float)y;

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

	glGenFramebuffers(1, &glowReduce);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, glowReduce);

	glowTexture.init(x, y, 0, false, false, false);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + 1, GL_TEXTURE_2D, glowTexture.getTargetId(), 0);

	GLenum glowTextureBuffers[2] = { GL_NONE, GL_COLOR_ATTACHMENT0 + 1 };
	glDrawBuffers(2, glowTextureBuffers);
	
	Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (Status != GL_FRAMEBUFFER_COMPLETE) {
		throw;
	}

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	if (!shaderPtr)
	{
		throw;
	}

	uniformBitsList = new GLuint[nrTextures];

	//ambient blitquad
	uniformEyePos = glGetUniformLocation(*shaderPtr, "eyepos");

	//spotlight volumes
	spotVolBufferPos = glGetUniformBlockIndex(spotVolShader, "Light");
	spotID = glGetUniformLocation(spotVolShader, "spotlightID");
	spotVolVP = glGetUniformLocation(spotVolShader, "ViewProjMatrix");
	spotVolEye = glGetUniformLocation(spotVolShader, "eyepos");
	spotvol_Position = glGetUniformLocation(spotVolShader, "Position");
	spotvol_Diffuse = glGetUniformLocation(spotVolShader, "Diffuse");
	spotvol_Normal = glGetUniformLocation(spotVolShader, "Normal");
	spotvol_GlowMap = glGetUniformLocation(spotVolShader, "GlowMap");

	glProgramUniform1i(spotVolShader, spotvol_Position, 1);
	glProgramUniform1i(spotVolShader, spotvol_Diffuse, 2);
	glProgramUniform1i(spotVolShader, spotvol_Normal, 3);
	glProgramUniform1i(spotVolShader, spotvol_GlowMap, 4);

	//pointlight volumes
	spotVolBuffer = glGetUniformBlockIndex(pointVolShader, "Light");
	pointID = glGetUniformLocation(pointVolShader, "pointlightID");
	pointVolVP = glGetUniformLocation(pointVolShader, "ViewProjMatrix");
	pointVolEye = glGetUniformLocation(pointVolShader, "eyepos");
	pointvol_Position = glGetUniformLocation(pointVolShader, "Position");
	pointvol_Diffuse = glGetUniformLocation(pointVolShader, "Diffuse");
	pointvol_Normal = glGetUniformLocation(pointVolShader, "Normal");
	pointvol_GlowMap = glGetUniformLocation(pointVolShader, "GlowMap");

	glProgramUniform1i(pointVolShader, pointvol_Position, 1);
	glProgramUniform1i(pointVolShader, pointvol_Diffuse, 2);
	glProgramUniform1i(pointVolShader, pointvol_Normal, 3);
	glProgramUniform1i(pointVolShader, pointvol_GlowMap, 4);

	uBlitLightPixelX  = glGetUniformLocation(*shaderPtr, "pixeluvX");
	uBlitLightPixelY = glGetUniformLocation(*shaderPtr, "pixeluvY");

	setGlowSamplingDist(1.0f);

	if (depth)
	{
		uniformBitsList[0] = glGetUniformLocation(*shaderPtr, "Depth");;
	}
	uniformBitsList[1] = glGetUniformLocation(*shaderPtr, "Position");
	uniformBitsList[2] = glGetUniformLocation(*shaderPtr, "Diffuse");
	uniformBitsList[3] = glGetUniformLocation(*shaderPtr, "Normal");
	uniformBitsList[4] = glGetUniformLocation(*shaderPtr, "GlowMap");;
	uniformUse = glGetUniformLocation(*shaderPtr, "Use");
	

	if (!portal_shaderPtr)
	{
		throw;
	}

	//portal shader init
	portal_vp = glGetUniformLocation(*portal_shaderPtr, "VP");
	portal_model = glGetUniformLocation(*portal_shaderPtr, "world");


	blitQuads = new BlitQuad[6];
	blitQuads[0].Init(vec3(-1, -1, 0), vec3(-0.6, -0.6, 0 ));
	blitQuads[1].Init(vec3(-0.6, -1, 0), vec3(-0.2, -0.6, 0));
	blitQuads[2].Init(vec3(-0.2, -1, 0), vec3(0.2, -0.6, 0));
	blitQuads[3].Init(vec3(0.2, -1, 0), vec3(0.6, -0.6, 0));
	blitQuads[4].Init(vec3(0.6, -1, 0), vec3(1, -0.6, 0));
	blitQuads[5].Init(vec3(-1, -1, 0), vec3(1, 1, 0));
	
	initialized = true;

	initLight();
}

void Gbuffer::initLight()
{
	if (targetId == 0)
		throw;
	glGenBuffers(1, &spotVolBuffer);
	glGenBuffers(1, &pointVolBuffer);

	maxLights = 500;

	glBindBuffer(GL_UNIFORM_BUFFER, spotVolBuffer);
	glBufferData(GL_UNIFORM_BUFFER, maxLights * sizeof(SpotLight), NULL, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_UNIFORM_BUFFER, pointVolBuffer);
	glBufferData(GL_UNIFORM_BUFFER, maxLights * sizeof(SpotLight), NULL, GL_DYNAMIC_DRAW);

	lightInitialized = true;
}

Gbuffer::~Gbuffer()
{
	delete[] rTexture;
	delete[] uniformBitsList;
	for (size_t i = 0; i < 6; i++)
	{
		blitQuads[i].release();
	}
	delete[] blitQuads;
	if (initialized)
	{
		glDeleteFramebuffers(1, &targetId);
		//Debug::DebugOutput("Deleting gbuffer target\n");
	}

	glDeleteBuffers(1, &spotVolBuffer);
	glDeleteBuffers(1, &pointVolBuffer);

	glDeleteShader(*shaderPtr);
	delete shaderPtr;
	glDeleteShader(*portal_shaderPtr);
	delete portal_shaderPtr;

	RenderTarget::releaseStatic();
}

void Gbuffer::setGlowSamplingDist(float dist)
{
	glProgramUniform1f(*shaderPtr, uBlitLightPixelX, dist / float(xres));
	glProgramUniform1f(*shaderPtr, uBlitLightPixelY, dist / float(yres));
}

void Gbuffer::resize(int x, int y)
{
	for (int i = 0; i < nrTextures; i++)
	{
		rTexture[i].resize(x, y);
	}
}

void Gbuffer::pushLights(SpotLight* light, int nrLight)
{
	//push spotlights
	if (length(light->Direction) > 0.5f)
	{
		glBindBuffer(GL_UNIFORM_BUFFER, spotVolBuffer);
		
		if (nrOfSpotLights + nrLight < maxLights)
		{
			glBindBuffer(GL_UNIFORM_BUFFER, spotVolBuffer);
			glBufferSubData(GL_UNIFORM_BUFFER, nrOfSpotLights * sizeof(SpotLight), nrLight * sizeof(SpotLight), &light[0]);
			nrOfSpotLights += nrLight;
		}
	}
	//push pointlights
	else 
	{
		glBindBuffer(GL_UNIFORM_BUFFER, pointVolBuffer);

		if (nrOfPointLights + nrLight < maxLights)
		{
			glBindBuffer(GL_UNIFORM_BUFFER, pointVolBuffer);
			glBufferSubData(GL_UNIFORM_BUFFER, nrOfPointLights * sizeof(SpotLight), nrLight * sizeof(SpotLight), &light[0]);
			nrOfPointLights += nrLight;
		}
	}
}

void Gbuffer::bind(GLuint target)
{
	glBindFramebuffer(target, targetId);
}

void Gbuffer::preRender(GLuint shader, GLuint texture, GLuint self)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, rTexture[4].getTargetId());
	glProgramUniform1i(shader, texture, 0);
	
	glActiveTexture(GL_TEXTURE0 + 1);
	glBindTexture(GL_TEXTURE_2D, glowTexture.getTargetId());
	glProgramUniform1i(shader, self, 1);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, glowReduce);

	blitQuads[5].BindVertData();

	glDisable(GL_DEPTH_TEST);
	//glEnable(GL_BLEND);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glEnable(GL_DEPTH_TEST);
	//glDisable(GL_BLEND);
}

void Gbuffer::render(/*glm::vec3 playerPos, glm::vec3 playerDir*/)
{
	// bind shader
	glUseProgram(*shaderPtr);

	// bind textures
	for (int i = 0; i < nrTextures; i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		//if (i == 4)
		//{
		//	glBindTexture(GL_TEXTURE_2D, glowTexture.getTargetId());
		//}
		//else
		//{
			glBindTexture(GL_TEXTURE_2D, rTexture[i].getTargetId());
		//}
		glProgramUniform1i(*shaderPtr, uniformBitsList[i], i);
	}

	glProgramUniform3fv(*shaderPtr, uniformEyePos, 1, &eyePos[0]);
	//glProgramUniform1i(*shaderPtr, uniformNrOfLight, nrOfLights);

	// bind buffer
	glBindBuffer(GL_ARRAY_BUFFER, renderQuad);
	glBindVertexArray(renderVao);

	blitQuads[5].BindVertData();
	glProgramUniform1i(*shaderPtr, uniformUse, 5);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	//each blit
	//for (int n = 0; n < 5; n++)
	//{
	//	blitQuads[n].BindVertData();
	//	glProgramUniform1i(*shaderPtr, uniformUse, n);
	//	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	//}
}

void Gbuffer::clearBuffers()
{
	float clear = 0.0f;
	glClear(GL_DEPTH_BUFFER_BIT);
	glClearBufferfv(GL_COLOR, 1, &clear);
	glClearBufferfv(GL_COLOR, 2, &clear);
	glClearBufferfv(GL_COLOR, 3, &clear);
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
			if(i == 1)
				rTexture[i].init(x, y, 0, false, true, true);	// wpos
			else if (i == 2)
				rTexture[i].init(x, y, 0, false, false, false);	// diffuse
			else if (i == 3)
				rTexture[i].init(x, y, 0, false, false, true);	// normal
			else if (i == 4)
				rTexture[i].init(x, y, 0, false, false, false);	// glow

		}
		
	}
}
