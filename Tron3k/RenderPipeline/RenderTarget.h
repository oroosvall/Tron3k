#ifndef RENDERTARGET_H
#define RENDERTARGET_H

#include "Utils\GPUMemoryLeakChecker.h"

class RenderTarget
{

public:

	virtual void init(int x, int y, int nrTex, bool useDepth);

	virtual ~RenderTarget();
	static void releaseStatic();

	virtual void resize(int x, int y);

	virtual void bind(GLuint target);

	virtual void render();

	GLuint getTargetId() const;

	static GLuint RenderTarget::renderQuad;
	static GLuint RenderTarget::renderVao;

protected:

	GLuint targetId;

	bool depth;
	int nrTextures;

	static void genQuad();

};

#endif
