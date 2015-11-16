#ifndef CONTENTMANAGER_H
#define CONTENTMANAGER_H

#include "ResourceManager.h"
#include <glm\glm.hpp>

#include <GL\glew.h>

struct Object
{
	GLuint meshID;
	GLuint textureID;
	GLuint normalID;
};

struct Chunk
{
	unsigned int objectCount;
	struct ObjectList
	{
		unsigned int objectID;
		unsigned int nrObjects;
		glm::mat4* worldMatPtr;
	} ;
};

struct PlayerObject
{
	GLuint meshID;
	GLuint index;
	GLuint vao;
	int facecount;
	GLuint textureID;
};

class ContentManager
{

private:
	
	Object* objects;
	Chunk* chunks;
	
	PlayerObject* playerModels;

public:

	void init();

	void renderPlayer(int playerID, glm::mat4 world);

	void loadChunk();
	void releaseChunk();


};

#endif