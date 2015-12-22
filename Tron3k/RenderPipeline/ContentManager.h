#ifndef CONTENTMANAGER_H
#define CONTENTMANAGER_H

#include <glm\glm.hpp>
#include "Mesh.h"
#include "Utils\GPUMemoryLeakChecker.h"
#include "Map\Map.h"
#include <vector>

#include "AnimatedMesh.h"
#include "../Project/Core/AnimationState.h"

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

	//trapper files
	//AnimatedMesh trapper_first_primary_idle;
	AnimatedMesh trapper_first_primary_run;
	//AnimatedMesh trapper_first_primary_air;

	AnimatedMesh trapper_first_primary_fire;
	AnimatedMesh trapper_first_primary_reload;
	AnimatedMesh trapper_first_primary_throw;
	AnimatedMesh trapper_first_primary_switch;

	AnimatedMesh trapper_first_secondary_fire;

	AnimatedMesh trapper_third_idle;
	AnimatedMesh trapper_third_run;

	AnimatedMesh trapper_third_jump_air;
	AnimatedMesh trapper_third_jump_begin;
	AnimatedMesh trapper_third_jump_end;
	AnimatedMesh trapper_third_strafe_left;
	AnimatedMesh trapper_third_strafe_right;


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