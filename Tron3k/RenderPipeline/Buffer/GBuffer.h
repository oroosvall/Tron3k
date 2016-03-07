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

	void preRender(GLuint shader, GLuint location);
	void render();

	void postProcessPass(GLuint shader, GLuint location, GLuint uvX, GLuint uvY, GLuint fxaalocation);

	void clearLights();
	void clearBuffers();
	void pushLights(SpotLight* light, int nrLight);

	void setGlowSamplingDist(float Dist);
	float xres;
	float yres;

	//Spotlight volume shader
	GLuint spotVolShader;
	GLuint spotVolVP;
	GLuint spotVolEye;
	GLuint spotID;

	GLuint spotvol_Position;
	GLuint spotvol_Diffuse;
	GLuint spotvol_Normal;
	GLuint spotvol_GlowMap;
	GLuint spotvol_Res;

	//spotLight Buffers
	GLuint spotVolBuffer;
	GLuint spotVolBufferPos; 

	//pointlight volume shader
	GLuint pointVolShader;
	GLuint pointVolVP;
	GLuint pointVolEye;
	GLuint pointID;

	GLuint pointvol_Position;
	GLuint pointvol_Diffuse;
	GLuint pointvol_Normal;
	GLuint pointvol_GlowMap;
	GLuint pointvol_Res;

	//pointlight Buffers
	GLuint pointVolBuffer;
	GLuint pointVolBufferPos;

	GLuint* shaderPtr;
	GLuint blightlightUniversalInten;
	GLuint spotVolUniversalInten;
	GLuint pointVolUniversalInten;

	GLuint* portal_shaderPtr;

	glm::vec3 eyePos;
	GLuint uniformEyePos;

	//Portal shader uniforms
	glm::vec3 eyePosLast;
	GLuint portal_vp;
	GLuint portal_model;

	int nrOfSpotLights = 0;
	int nrOfPointLights = 0;

	GLuint glowSampleFbo;
	RenderTarget glowSampleTexture;

	GLuint postProcess;
	RenderTarget postProcessTexture;

	bool fxaa = false;

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