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

#include "TextureManager.h"

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
	
	//new bullet meshes
	MiscMesh manipConsume;
	MiscMesh manipMobil;
	MiscMesh manipPrim;
	MiscMesh manipSecon;

	MiscMesh puniConsume;
	MiscMesh puniPrim;
	MiscMesh puniSecon;

	MiscMesh stalkConsume;
	MiscMesh stalkPrim;

	MiscMesh trapConsumeBig;
	MiscMesh trapConsimeSmall;
	MiscMesh trapPrim;

	//pickups
	MiscMesh healthAmmoMesh;
	MiscMesh doubleDamageMesh;

	//effect mehshes
	MiscMesh explosionMesh;
	MiscMesh thunderDomeMesh;
	MiscMesh manipThunderMesh;
	MiscMesh deathPickupMesh;

	struct TriangleVertex
	{
		float pos[5];
	};

	TextureManager tm;

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
	bool f_render_roombox;
	bool f_render_gui;

	//minimap
	GLuint miniMapTexture;
	float minimapscaleX;
	float minimapScaleY;
	GLuint youarehereTexture;
	float youareherescaleX;
	float youareherescaleY;
	GLuint teamishereTexture;
	GLuint activeCapTexture;
	vec2 activeCapScale;
	vec2 mapBotcord;
	vec2 mapTopcord;

	//help texutre overlays
	GLuint help1;
	GLuint help2;
	GLuint help3;

	//take damage texture overlay
	GLuint takeDamage1;
	GLuint takeDamage2;
	GLuint takeDamage3;

	//scoreboard
	GLuint score_team_green;
	GLuint score_team_orange;
	GLuint score_header_green;
	GLuint score_header_orange;
	vec2 score_teamscale;
	vec2 score_headerscale;

	//trailquad textures
	unsigned int trailTex1;

	~ContentManager();
	void init();

	void release();

	void update(float dt);

	void renderParticles(GLuint shader, GLuint textureLoc, GLuint particleSize, GLuint glowLoc, GLuint scaleDir);
	void renderChunks(GLuint shader, GLuint shaderLocation, GLuint textureLocation, GLuint normalLocation, GLuint glowSpecLocation, GLuint DglowColor, GLuint SglowColor, GLuint collision_portal_shader, GLuint collision_portal_world, GLuint portal, GLuint portal_world);

	void renderBullet(int bid);
	void renderEffect(int eid);

	void renderMisc(int renderID, GLuint shader, GLuint textureLocation, GLuint normalLocation, GLuint glowSpecLocation);

	void renderCapturePoint(int id, GLuint shader, GLuint shaderLocation, GLuint diffuseLocation, GLuint normalLocation, GLuint glowLocation);

	void renderPlayer(AnimManager::animState, glm::mat4 world, GLuint uniformKeyMatrixLocation, bool first, bool primary, GLuint shader, GLuint textureLocation, GLuint normalLocation, GLuint glowSpecLocation);

	int getPortalID(glm::vec3* oldpos, glm::vec3* newpos, int in_chunk) { return testMap.portalintersection(oldpos, newpos, in_chunk); }

	void* getChunkCollisionVectorAsPointer(int chunkID);
	void* getCapAsPointer(int& count);
	void* getRoomBoxes();

	void bindLightwalTexture(GLuint shader, GLuint location);

	void bindDecalTexture();

	std::vector<std::vector<float>> getMeshBoxes(); //Will send out all meshboxes, for now it'll just send a static one

	void setRoomID(int roomID);
};

#endif