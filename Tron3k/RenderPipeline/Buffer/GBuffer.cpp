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

	glGenFramebuffers(1, &glowSampleFbo);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, glowSampleFbo);

	glowSampleTexture.init(x, y, 0, false, false, false);

	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, glowSampleTexture.getTargetId(), 0);

	GLenum glowDrawBuffer[] = { GL_NONE, GL_COLOR_ATTACHMENT0 };

	glDrawBuffers(2, glowDrawBuffer);

	Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (Status != GL_FRAMEBUFFER_COMPLETE) {
		throw;
	}
	
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	glGenFramebuffers(1, &postProcess);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, postProcess);

	postProcessTexture.init(x, y, 0, false, false, false);

	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, postProcessTexture.getTargetId(), 0);

	//GLenum glowDrawBuffer[] = { GL_NONE, GL_COLOR_ATTACHMENT0 };

	glDrawBuffers(2, glowDrawBuffer);

	Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (Status != GL_FRAMEBUFFER_COMPLETE) {
		throw;
	}

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	if (!shaderPtr)
	{
		throw;
	}

	uniformBitsList = new GLuint[nrTextures+1];

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
	spotvol_Res = glGetUniformLocation(pointVolShader, "gScreenSize");

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
	pointvol_Res = glGetUniformLocation(pointVolShader, "gScreenSize");

	glProgramUniform1i(pointVolShader, pointvol_Position, 1);
	glProgramUniform1i(pointVolShader, pointvol_Diffuse, 2);
	glProgramUniform1i(pointVolShader, pointvol_Normal, 3);
	glProgramUniform1i(pointVolShader, pointvol_GlowMap, 4);

	uBlitLightPixelX  = glGetUniformLocation(*shaderPtr, "pixeluvX");
	uBlitLightPixelY = glGetUniformLocation(*shaderPtr, "pixeluvY");

	blightlightUniversalInten = glGetUniformLocation(*shaderPtr, "universalInten");
	spotVolUniversalInten = glGetUniformLocation(spotVolShader, "universalInten");
	pointVolUniversalInten = glGetUniformLocation(pointVolShader, "universalInten");

	glProgramUniform1f(*shaderPtr, blightlightUniversalInten, 1.0f);
	glProgramUniform1f(spotVolShader, spotVolUniversalInten, 1.0f);
	glProgramUniform1f(pointVolShader, pointVolUniversalInten, 1.0f);


	glProgramUniform2f(pointVolShader, pointvol_Res, xres, yres);
	glProgramUniform2f(spotVolShader, spotvol_Res, xres, yres);


	setGlowSamplingDist(1.0f);

	if (depth)
	{
		uniformBitsList[0] = glGetUniformLocation(*shaderPtr, "Depth");;
	}
	uniformBitsList[1] = glGetUniformLocation(*shaderPtr, "Position");
	uniformBitsList[2] = glGetUniformLocation(*shaderPtr, "Diffuse");
	uniformBitsList[3] = glGetUniformLocation(*shaderPtr, "Normal");
	uniformBitsList[4] = glGetUniformLocation(*shaderPtr, "GlowMap");
	uniformBitsList[5] = glGetUniformLocation(*shaderPtr, "GlowMap2");
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

void Gbuffer::preRender(GLuint shader, GLuint location)
{
	glUseProgram(shader);
	glActiveTexture(GL_TEXTURE0);
	glProgramUniform1i(shader, location, 0);
	glBindTexture(GL_TEXTURE_2D, rTexture[4].getTargetId());

	glBindFramebuffer(GL_FRAMEBUFFER, glowSampleFbo);

	blitQuads[5].BindVertData();
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

}

void Gbuffer::render()
{


	//glBindFramebuffer(GL_FRAMEBUFFER, NULL);
	//glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	glBindFramebuffer(GL_FRAMEBUFFER, postProcess);
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	
	// bind shader
	glUseProgram(*shaderPtr);
	
	// bind textures
	for (int i = 0; i < nrTextures; i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		//if (i == 4)
		//{
		//	glBindTexture(GL_TEXTURE_2D, glowSampleTexture.getTargetId());
		//}
		//else
		//{
		glBindTexture(GL_TEXTURE_2D, rTexture[i].getTargetId());
		//}
		glProgramUniform1i(*shaderPtr, uniformBitsList[i], i);
	}
	glActiveTexture(GL_TEXTURE0 + 6);
	glBindTexture(GL_TEXTURE_2D, glowSampleTexture.getTargetId());
	glProgramUniform1i(*shaderPtr, uniformBitsList[5], 6);
	
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

void Gbuffer::postProcessPass(GLuint shader, GLuint location, GLuint uvX, GLuint uvY, GLuint fxaalocation)
{

	glBindFramebuffer(GL_FRAMEBUFFER, NULL);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	glUseProgram(shader);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, postProcessTexture.getTargetId());
	glProgramUniform1i(shader, location, 0);
	glProgramUniform1i(shader, fxaalocation, fxaa);
	glProgramUniform1f(shader, uvX, 1.0f / xres);
	glProgramUniform1f(shader, uvY, 1.0f / yres);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

}

void Gbuffer::clearBuffers()
{
	float clear = 0.0f;
	glClear(GL_DEPTH_BUFFER_BIT);
	glClearBufferfv(GL_COLOR, 1, &clear);
	glClearBufferfv(GL_COLOR, 2, &clear);
	glClearBufferfv(GL_COLOR, 3, &clear);
	glClearBufferfv(GL_COLOR, 4, &clear);
	glClearBufferfv(GL_COLOR, 5, &clear);
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
			else
				rTexture[i].init(x, y, 0, false, false, false);	// glow

		}
		
	}
}
