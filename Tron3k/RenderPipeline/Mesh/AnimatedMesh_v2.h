#ifndef ANIMATEDMESH_V2_H
#define ANIMATEDMESH_V2_H

#include <string>
#include <glm\glm.hpp>
#include "..\Map\MapHeaders.h"
#include "../Utils/GPUMemoryLeakChecker.h"

class AnimatedMeshV2
{
private:

	int* matOffsets;
	int* indices;
	AnimVertex* verts;

	GLuint vbo;
	GLuint ibo;
	GLuint vao;

	int indexCount;

	TextureStruct* tex;
	int textureCount;

	Material* materials;

	// todo add animation states & animation data

public:

	void init();
	void release();

	~AnimatedMeshV2();

	void load(std::string fileName);

	void draw(GLuint uniformKeyMatrixLocation);

};


#endif