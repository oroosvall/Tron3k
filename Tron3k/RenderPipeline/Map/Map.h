#ifndef MAP_H
#define MAP_H

#include "../StaticMesh.h"
#include "MapHeaders.h"
#include "../Texture.h"
#include <string>

struct TextureStruct
{
	std::string textureName;
	GLuint textureID;
	GLuint fileTexID;
};

class Map
{

private:

	Material* materials;
	
	TextureStruct* tex;

	BBPoint* bbPoints;
	StaticMesh* meshes;

	int meshCount;
	int textureCount;

public:
	
	void init();
	void release();

	void render(GLuint shader, GLuint shaderLocation, GLuint texture, GLuint normal, GLuint spec);

	void loadMap(std::string mapName);

};

#endif