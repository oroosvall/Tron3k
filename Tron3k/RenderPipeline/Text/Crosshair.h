#ifndef CROSSHAIR_H
#define CROSSHAIR_H

#include "../Utils/GPUMemoryLeakChecker.h"
#include "../Manager/TextureManager.h"

#include "Text.h"

class Crosshair
{

private:

	GLuint vbo;
	GLuint vao;

public:

	void init(int, int);

	void release();
	
	void draw();


};

#endif