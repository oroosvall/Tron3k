#ifndef CONTENTMANAGER_H
#define CONTENTMANAGER_H

#include <glm\glm.hpp>
#include "Mesh.h"
#include "Utils\GPUMemoryLeakChecker.h"
#include "Map\Map.h"
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

	//skybox
	Mesh skybox;

	Mesh bullet;

	GLuint portalQuery;
public:
	Map testMap;

	int nrChunks;
	bool* renderedChunks;
	bool* renderNextChunks;

	~ContentManager();
	void init();

	void release();

	void renderChunks(GLuint shader, GLuint shaderLocation, GLuint textureLocation, GLuint normalLocation, GLuint glowSpecLocation, GLuint DglowColor, GLuint SglowColor, GLuint portal_shader, GLuint portal_world);

	void renderPlayer(int playerID, glm::mat4 world);

	int getPortalID(glm::vec3 oldpos, glm::vec3 newpos) { return testMap.getChunkID(oldpos, newpos); }

	void renderPortals(GLuint shader, GLuint world);

	void loadChunk();
	void releaseChunk();

	void* getChunkCollisionVectorAsPointer(int chunkID);

	std::vector<std::vector<float>> getMeshBoxes(); //Will send out all meshboxes, for now it'll just send a static one
};

#endif