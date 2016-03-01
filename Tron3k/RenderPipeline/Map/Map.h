#ifndef MAP_H
#define MAP_H

#include "../Mesh/StaticMesh.h"
#include "MapHeaders.h"
#include "../Texture.h"
#include <glm\glm.hpp>
#include <string>
#include "../BlitQuad.h" //used for rendering portals
#include <vector>

#include "../Manager/TextureManager.h"

using std::vector;

extern GLuint compute;
extern ParticleShaderLocations locations;

struct CapturePoint
{
	int roomID;

	ABB bigAABB;
	CollisionRenderABB renderBigAbb;
	ABB* aabb;
	CollisionRenderABB* renderAbb;
	StaticMesh* meshes;

	glm::mat4* mats;

	glm::vec3 color;

	int meshCount;
	int aabbCount;

	void buildABBS();
	void release();

	CapturePoint()
	{
		int roomID = 0;
		int meshCount = 0;
		int aabbCount = 0;
	}

};

struct CaptureExportToGame
{
	int roomID;
	ABB bigAABB;
	int subcount;
	ABB* subabbs;
};

class Map
{

private:
	Material* materials;
	ABB* chunkAABB;
	TextureStruct* tex;

	BBPoint* bbPoints;
	StaticMesh* meshes;

	int* capPointAABBCount;
	int* capPointWallCount;
	CapturePoint* capturePoints;

	int meshCount;
	int textureCount;
	int materialCount;
	int capCount;

	int bbCount;

public:

	TextureManager* tm;

	vector<Chunk> chunks;

	ParticleSystem_sdf* particleStuff;
	int particleCount;

	int nrTotalStaticSpotlights = 0;
	int nrTotalStaticPointlights = 0;

	int currentChunk;
	int roomCount;

	SpawnPoint* spA;
	SpawnPoint* spB;
	SpawnPoint* spFFA;
	int spTACount;
	int spTBCount;
	int spFFACount;

	void init(TextureManager* _tm);
	void release();

	void update(float dt);
	void renderParticles(GLuint shader, GLuint textureLoc, GLuint particleSize, GLuint glowLoc, GLuint scaleDir);

	void renderChunk(GLuint shader, GLuint shaderLocation, GLuint diffuseLocation, GLuint normalLocation, GLuint glowLocation, int chunkID);

	void renderCapturePoint(GLuint shader, GLuint shaderLocation, GLuint diffuseLocation, GLuint normalLocation, GLuint glowLocation, int capturePointID);

	void renderCapAbb();

	void loadMap(std::string mapName);

	int portalintersection(glm::vec3* oldPos, glm::vec3* newPos, int in_currentchunk);

	ChunkCollision* getChunkCollision(int chunkID);
	void* getCapAsPointer(int& count);
	void* getRoomBoxes() { return chunkAABB; };
	
	int getCapPointRoomID(int capPoint);
	void setCapPointColor(int capID, glm::vec3 color);

	void* getCapPointColor(int cap);

	void deleteSpawnposData();
};

#endif