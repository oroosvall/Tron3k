#ifndef GBUFFER_H
#define GBUFFER_H

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include "RenderTarget.h"
#include "..\Lights.h"

#include "..\BlitQuad.h"

class Gbuffer : public RenderTarget
{
public:
	Gbuffer();
	~Gbuffer();

	void init(int x, int y, int nrTex, bool depth);
	void initLight();

	void resize(int x, int y);

	void bind(GLuint index);

	void preRender(GLuint shader, GLuint texture, GLuint self);
	void render(/*glm::vec3 playerPos, glm::vec3 playerDir*/);

	void clearLights();
	void clearBuffers();
	void pushLights(SpotLight* light, int nrLight);

	void setGlowSamplingDist(float Dist);
	float xres;
	float yres;

	//Spotlight volume shader
	GLuint spotligtVolumeShader;
	GLuint spotlightVP;
	GLuint uniformEyePos_spotlightVolume;
	GLuint spotID;

	GLuint spotvol_Position;
	GLuint spotvol_Diffuse;
	GLuint spotvol_Normal;
	GLuint spotvol_GlowMap;

	//Light Buffers
	GLuint lightBuffer_spotlightVolume;
	GLuint uniformBufferLightPos_spotlightVolume; 

	GLuint* shaderPtr;
	GLuint* portal_shaderPtr;

	//Light variables
	int nrOfLights;
	glm::vec3 eyePos;
	GLuint uniformEyePos;

	//Portal shader uniforms
	glm::vec3 eyePosLast;
	GLuint portal_vp;
	GLuint portal_model;

	GLuint glowReduce;

	RenderTarget glowTexture;

	

private:

	bool initialized;
	bool lightInitialized;

	void generate(int x, int y);

	RenderTarget* rTexture;

	//Uniforms
	GLuint* uniformBitsList;
	GLuint uniformUse;

	GLuint uBlitLightPixelX;
	GLuint uBlitLightPixelY;

	//Bits
	BlitQuad* blitQuads;

	int maxLights;

};

#endif