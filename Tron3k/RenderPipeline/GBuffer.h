#ifndef GBUFFER_H
#define GBUFFER_H

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include "RenderTarget.h"
#include "Lights.h"

#include "BlitQuad.h"

class Gbuffer : public RenderTarget
{
public:
	Gbuffer();
	~Gbuffer();

	void init(int x, int y, int nrTex, bool depth);
	void initLight();

	void resize(int x, int y);

	void bind(GLuint index);

	void preRender(GLuint shader, GLuint texture);
	void render(/*glm::vec3 playerPos, glm::vec3 playerDir*/);

	void clearLights();
	void clearBuffers();
	void pushLights(SpotLight* light, int nrLight);

	GLuint* shaderPtr;
	GLuint* portal_shaderPtr;

	//Light variables
	int nrOfLights;
	glm::vec3 eyePos;

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
	GLuint uniformEyePos;
	GLuint uniformNrOfLight;

	GLuint uBlitLightPixelX;
	GLuint uBlitLightPixelY;

	//Light Buffers
	GLuint lightBuffer;
	GLuint uniformBufferLightPos;

	//Bits
	BlitQuad* blitQuads;

	int maxLights;

};

#endif