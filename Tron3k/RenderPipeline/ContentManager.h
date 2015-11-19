#ifndef CONTENTMANAGER_H
#define CONTENTMANAGER_H

#include <glm\glm.hpp>
#include "Mesh.h"
#include <GL\glew.h>

#include <vector>

struct PlayerObject
{
	GLuint meshID;
	GLuint index;
	GLuint vao;
	int facecount;
	GLuint textureID;
};

struct TextureLookup
{
	std::string textureName;
	GLuint textureID;
	GLuint fileTexID;
	bool loaded;
};

class ContentManager
{

private:

	std::vector<Mesh> meshes;
	std::vector<TextureLookup> textures;
	PlayerObject* playerModels;

public:
	~ContentManager();
	void init();

	void renderChunks(GLuint shader, GLuint shaderLocation, GLuint textureLocation);

	void renderPlayer(int playerID, glm::mat4 world);

	void loadChunk();
	void releaseChunk();


};

#endif