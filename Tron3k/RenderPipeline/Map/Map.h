#ifndef MAP_H
#define MAP_H

#include "../Mesh/StaticMesh.h"
#include "MapHeaders.h"
#include "../Texture.h"
#include <glm\glm.hpp>
#include <string>
#include "../BlitQuad.h" //used for rendering portals
#include <vector>

using std::vector;

class Map
{

private:

	Material* materials;

	
	TextureStruct* tex;

	BBPoint* bbPoints;
	StaticMesh* meshes;

	int meshCount;
	int textureCount;
	int materialCount;

	int bbCount;

public:
	vector<Chunk> chunks;
	int currentChunk;
	int roomCount;

	SpawnPoint* spA;	
	SpawnPoint* spB;	
	SpawnPoint* spFFA;	
	int spTACount;
	int spTBCount;
	int spFFACount;

	void init();
	void release();

	void renderChunk(GLuint shader, GLuint shaderLocation, int chunkID);

	void loadMap(std::string mapName);

	int portalintersection(glm::vec3* oldPos, glm::vec3* newPos, int in_currentchunk);

	ChunkCollision* getChunkCollision(int chunkID);

	void deleteSpawnposData();
};

#endif