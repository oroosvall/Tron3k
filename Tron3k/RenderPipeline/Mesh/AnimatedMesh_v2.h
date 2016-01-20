#ifndef ANIMATEDMESH_V2_H
#define ANIMATEDMESH_V2_H

#include <string>
#include <glm\glm.hpp>
#include "..\Map\MapHeaders.h"
#include "../Utils/GPUMemoryLeakChecker.h"

#include "AnimationData.h"
#include "../../Project/Core/AnimationState.h"


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

	GLuint matricesBuffer;
	AnimData animations[AnimationState::none];
	// todo add animation states & animation data

public:

	void init();
	void release();

	~AnimatedMeshV2();

	void update();

	void load(std::string fileName);
	int* loadAnimations(std::string character);

	void draw(GLuint uniformKeyMatrixLocation, int animationID, int keyFrame);

};


#endif