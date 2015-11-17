#ifndef GBUFFER_H
#define GBUFFER_H

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include "RenderTarget.h"

#include "BlitQuad.h"

class Gbuffer : public RenderTarget
{
public:
	Gbuffer();
	~Gbuffer();

	void init(int x, int y, int nrTex, bool depth);
	void resize(int x, int y);

	void bind(GLuint index);

	void render();

	GLuint* shaderPtr;
private:

	bool initialized;

	void generate(int x, int y);

	RenderTarget* rTexture;

	GLuint* pos;

	BlitQuad* blitQuads;
};

#endif