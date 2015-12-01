#ifndef MAP_H
#define MAP_H

#include "../StaticMesh.h"
#include "MapHeaders.h"
#include "../Texture.h"
#include <glm\glm.hpp>
#include <string>
#include "../BlitQuad.h" //used for rendering portals
#include <vector>

using std::vector;

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

	int currentChunk;

	vector<Chunk> chunks;

	vector<PortalData> portals;
public:
	
	void init();
	void release();

	void render(GLuint shader, GLuint shaderLocation, GLuint texture, GLuint normal, GLuint spec);

	void loadMap(std::string mapName);

	int getChunkID(glm::vec3 oldPos, glm::vec3 newPos);

	void renderPortals();

};

#endif