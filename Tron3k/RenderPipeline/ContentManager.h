#ifndef CONTENTMANAGER_H
#define CONTENTMANAGER_H

#include <glm\glm.hpp>
#include "Mesh.h"
#include "Utils\GPUMemoryLeakChecker.h"
#include "Map\Map.h"
#include <vector>

#include "AnimatedMesh.h"

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

	AnimatedMesh trapepr_Run_Third;
	AnimatedMesh trapper_Reload_First;

	struct TriangleVertex
	{
		float pos[5];
	};
public:
	Map testMap;

	int nrChunks;
	bool* renderedChunks;
	bool* renderNextChunks;

	//Flags
	bool f_portal_culling;
	bool f_freeze_portals;
	bool f_render_chunks;
	bool f_render_abb;
	bool f_render_obb;

	~ContentManager();
	void init();

	void release();

	void renderChunks(GLuint shader, GLuint shaderLocation, GLuint textureLocation, GLuint normalLocation, GLuint glowSpecLocation, GLuint DglowColor, GLuint SglowColor, GLuint portal_shader, GLuint portal_world);

	void renderPlayer(int renderID, int keyframe, glm::mat4 world, GLuint uniformKeyMatrixLocation);

	int getPortalID(glm::vec3 oldpos, glm::vec3 newpos) { return testMap.getChunkID(oldpos, newpos); }

	void renderPortals(GLuint shader, GLuint world);

	void loadChunk();
	void releaseChunk();

	void* getChunkCollisionVectorAsPointer(int chunkID);
	void bindLightwalTexture();

	std::vector<std::vector<float>> getMeshBoxes(); //Will send out all meshboxes, for now it'll just send a static one
};

#endif