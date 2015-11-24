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

	void render(/*glm::vec3 playerPos, glm::vec3 playerDir*/);

	void clearLights();
	void pushLights(SpotLight* light, int nrLight);

	GLuint* shaderPtr;

	//Light variables
	int nrOfLights;
	glm::vec3 eyePos;
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

	//Light Buffers
	GLuint lightBuffer;
	GLuint uniformBufferLightPos;

	//Bits
	BlitQuad* blitQuads;

	int maxLights;
};

#endif