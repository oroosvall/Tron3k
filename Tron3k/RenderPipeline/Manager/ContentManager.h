#ifndef CONTENTMANAGER_H
#define CONTENTMANAGER_H

#include <glm\glm.hpp>
#include "..\Mesh\Mesh.h"
#include "..\Utils\GPUMemoryLeakChecker.h"
#include "..\Map\Map.h"
#include <vector>

#include "..\Mesh\AnimatedMesh.h"
#include "..\Mesh\AnimatedMesh_v2.h"
#include "../../Project/Core/AnimationState.h"
#include "../../Project/Core/Game/GameDataIndex.h"

#include "AnimManager.h"

#include "../Mesh/MiscMesh.h"

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
	//std::vector<TextureLookup> textures;

	//skybox
	Mesh skybox;

	GLuint lightWallTex;
	GLuint decalTexture;
	GLuint skyTexture;

	GLuint portalQuery;

	//character files
	AnimatedMeshV2 playerCharacters[5];

	// misc files

	MiscMesh trapperBullet;
	MiscMesh trapperConsume;
	MiscMesh shankerBullet;
	MiscMesh shankerSpecial;
	MiscMesh bruteThunderDome;

	struct TriangleVertex
	{
		float pos[5];
	};
public:
	Map testMap;

	int nrChunks;

	int keyFrameLengths[5 * AnimationState::none];

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

	void renderBullet(int bid);
	void renderThunderDome();
	void renderMisc(int renderID);

	void renderPlayer(AnimManager::animState, glm::mat4 world, GLuint uniformKeyMatrixLocation, bool first);

	int getPortalID(glm::vec3* oldpos, glm::vec3* newpos, int in_chunk) { return testMap.portalintersection(oldpos, newpos, in_chunk); }

	void* getChunkCollisionVectorAsPointer(int chunkID);
	void* getCapAsPointer(int& count);
	void* getRoomBoxes();

	void bindLightwalTexture();

	void bindDecalTexture();

	std::vector<std::vector<float>> getMeshBoxes(); //Will send out all meshboxes, for now it'll just send a static one

	void setRoomID(int roomID);
};

#endif