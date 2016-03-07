#ifndef IENGINE_H
#define IENGINE_H

#ifdef RENDERPIPELINE_EXPORTS
#define ENGINE_API __declspec( dllexport)
#else
#define ENGINE_API __declspec( dllimport  )
#endif

#include <string>
#include "Lights.h"
#include <vector>
#include "../Project/Core/sharedStructs.h"
#include "../Project/Core/AnimationState.h"

enum PIPELINE_SETTINGS
{
	CLEAR_COLOR,
	GLOW,
	SHADOWS,
	AO,
	VIEWPORT,
	REDUCEPARTICLES,
	DOF,
	DOF_VALUE,
	FOV
};

enum RENDER_FLAGS
{
	PORTAL_CULLING,
	FREEZE_CULLING,
	RENDER_CHUNK,
	RENDER_ABB,
	RENDER_OBB,
	RENDER_ROOM,
	RENDER_DEBUG_TEXT,
	RENDER_GUI,
	RENDER_FXAA,
};

enum CROSSHAIR_TYPE
{
	CROSSHAIR_TRAPPER_P,
	CROSSHAIR_SHANKER_P,
	CROSSHAIR_SHANKER_S,
	CROSSHAIR_BRUTE_P,
	CROSSHAIR_BRUTE_S,

	CROSSHAIR_NONE,
};

struct PipelineValues
{
	enum {INT2, BOOL, FLOAT, FLOAT3} type;
	union
	{
		bool bValue;
		float fValue;
		float xyz[3];
		int xy[2];
	};
};

enum BULLET_TYPE;
enum EFFECT_TYPE;

class IRenderPipeline
{
public:
	virtual bool init(unsigned int WindowWidth, unsigned int WindowHeight) = 0;
	virtual void release() = 0;
	virtual void update(float x, float y, float z, float dt) = 0;
	virtual void renderIni() = 0;

	virtual void reloadShaders() = 0;

	virtual void render() = 0;
	virtual void finalizeRender() = 0;

	virtual void addLight(SpotLight* newLight, int roomID) = 0;
	virtual void setChunkColorAndInten(int ID, float* color, float inten) = 0;

	virtual void* getView() = 0;
	virtual void renderMISC(int miscID, void* world, float* dgColor, float sgInten) = 0;
	virtual void renderBullet(int bid, void* world, float* dgColor, float sgInten) = 0;
	virtual void renderAnimation(int playerID, int roleID, void* world, AnimationState animState, float* dgColor, float sgInten, bool first, bool primary, int roomID) = 0;

	virtual std::string getStatus() = 0;

	PipelineValues getSettings(PIPELINE_SETTINGS type);
	virtual bool setSetting(PIPELINE_SETTINGS type, PipelineValues value) = 0;

	virtual int createMappedParticleEffect(BULLET_TYPE peffect, glm::vec3 pos, glm::vec3 dir, glm::vec3 color) = 0;
	virtual bool moveMappedParticleEffect(int id, glm::vec3 newPos) = 0;
	virtual void removeMappedParticleEffect(int id) = 0;

	virtual void createTimedParticleEffect(BULLET_TYPE peffect, glm::vec3 pos, glm::vec3 dir, glm::vec3 color) = 0;
	virtual void createTimedParticleEffect(EFFECT_TYPE eeffect, glm::vec3 pos, glm::vec3 color) = 0;

	virtual void renderWallEffect(void* pos1, void* pos2, float uvStartOffset, float* dgColor) = 0;
	
	virtual void initRenderRegular() = 0;
	virtual void initRenderEffect() = 0;
	virtual void rendereffect(int type, float* pos, float rad, float transp, float* dgColor) = 0;

	virtual void initRenderTrailQuad() = 0;
	virtual void renderTrailQUad(BULLET_TYPE type, float* pos, float* dir, float* crossdir, float* color) = 0;
	virtual void renderDecals(void* data, int size) = 0;

	virtual void renderCrosshair(CROSSHAIR_TYPE cross) = 0;

	virtual void renderCapturePoint(int capPointID) = 0;

	virtual void forceReset() = 0;

	virtual void setChatHistoryText(std::string text) = 0;
	virtual void setChatTypeMessage(std::string text) = 0;

	virtual int createTextObject(std::string text, int fontSize, glm::vec2 pos, bool scale = false) = 0;
	virtual void setTextObjectText(int id, std::string text) = 0;
	virtual void setTextPos(int id, glm::vec2 pos) = 0;
	virtual void removeTextObject(int id) = 0;
	virtual void renderTextObject(int id) = 0;
	virtual void renderTextObjectWorldPos(int id, glm::mat4 world) = 0;

	virtual void renderChatText() = 0;

	virtual void setCapRoomColor(int capPoint, glm::vec3 color, float intensity) = 0;

	virtual void setGBufferWin(unsigned int WindowWidth, unsigned int WindowHeight) = 0;

	virtual void* getChunkCollisionVectorAsPoint(int chunkID) = 0;
	virtual void* getCapPointsAsPoint(int& count) = 0;
	virtual void* getRoomBoxes() = 0;

	virtual void getPlayerBox(float &xMax, float &xMin, float &yMax, float &yMin, float &zMax, float &zMin) = 0;
	virtual void getWorldBoxes(int &current, float &xMax, float &xMin, float &yMax, float &yMin, float &zMax, float &zMin) = 0;
	virtual int getNrOfWorldBoxes() = 0;
	virtual void setRenderFlag(RENDER_FLAGS flag) = 0;
	virtual void getSpawnpoints(std::vector < std::vector < SpawnpointG > > &spoints) = 0;

	virtual void ui_initRender() = 0;
	virtual void ui_InGameRenderInit() = 0;
	virtual void ui_loadTexture(unsigned int* texid, char* filepath, int* xres, int* yres) = 0;
	virtual void ui_renderQuad(float* mat, float* pivot, unsigned int textureID, float transp, int i) = 0;
	virtual void ui_textureRelease(std::vector<unsigned int> texids) = 0;
	virtual int portalIntersection(float* pos1, float* pos2, int in_chunk) = 0;
	virtual void setCullingCurrentChunkID(int roomID) = 0;

	virtual void enableDepthTest() = 0;
	virtual void disableDepthTest() = 0;
	virtual void enableDepthMask() = 0;
	virtual void disableDepthMask() = 0;
	virtual void enableBlend(bool addetive) = 0;
	virtual void disableBlend() = 0; 
	virtual void clearBothBuffers() = 0;
	virtual void clearColor() = 0;

	virtual int startExecTimer(std::string name) = 0;
	virtual void stopExecTimer(int id) = 0;
	virtual bool* getRenderedChunks(int& get_size) = 0;

	virtual void renderTakeDamageDistort() = 0;
	virtual void startTakeDamageEffect(int maxDisplace, float time) = 0;
	virtual void setuniversalInten(float val) = 0;
	virtual void renderMinimap(float* yourPos, float* yourdir, float* teammates, int nrOfTeammates, int activeCap) = 0;
	virtual void renderScoreBoard(int team1size, int team2size) = 0;
	virtual void renderHelp(int id) = 0;

};

extern "C" ENGINE_API IRenderPipeline* CreatePipeline();

#endif