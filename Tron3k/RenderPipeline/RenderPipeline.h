#ifndef ENGINE_H
#define ENGINE_H

#include "IRenderPipeline.h"
#include "Text\Text.h"
#include "Camera.h"
#include "Buffer\GBuffer.h"

#include "Manager\ContentManager.h"

#include "Manager\AnimManager.h"

#include "Text\Crosshair.h"

#include "ParticleSystem\ParticleSystem.h"

enum SETTING_INPUT
{
	CLEAR_COLOR_IN = PipelineValues::FLOAT3,
	GLOW_IN = PipelineValues::BOOL,
	SHADOWS_IN = PipelineValues::BOOL,
	AO_IN = PipelineValues::BOOL,
	VIEWPORT_IN = PipelineValues::INT2,
	REDUCEPARTICLES_IN = PipelineValues::BOOL,
	DOF_IN = PipelineValues::BOOL,
	DOF_VALUE_IN = PipelineValues::FLOAT,
	FOV_IN = PipelineValues::FLOAT,

	NONE_IN = -1,
};

class RenderPipeline : public IRenderPipeline
{
private:

	SETTING_INPUT getType(PIPELINE_SETTINGS type) const;

	ContentManager contMan;
	AnimManager anims;

	//LightWall lw;

	Text* debugText;
	Text* chatText;

	GLuint fontTexture;

	string chatHistoryText;
	string chatTypeText;

	GLuint textShader;
	GLuint textShaderLocation;
	GLuint textShaderVP;
	GLuint textShaderModel;
	GLuint textShaderOffset;

	Camera cam;

	Gbuffer* gBuffer;

	GLuint regularShader;
	GLuint animationShader;
	GLuint glowShaderTweeks;

	GLuint uiShader;
	GLuint ui_Texture;
	GLuint ui_World;

	GLuint decal_Shader;
	//GLuint decal_struct_UBO;
	//GLuint decal_struct_UBO_index;
	//GLuint decal_Uniformtexsample;
	//GLuint decal_nrDecals;
	GLuint decal_viewProj;
	GLuint decal_pos;
	GLuint decal_normal;
	GLuint decal_color;
	GLuint decal_inten;

	GLuint portalShaderV2;
	GLuint portal_World;
	GLuint portal_VP;

	BlitQuad uiQuad;

	GLuint worldMat[2];
	GLuint viewProjMat[2];
	GLuint viewMat;

	GLuint uniformPivotLocation;
	GLuint uniformTextureLocation[2];
	GLuint uniformNormalLocation[2];
	GLuint uniformGlowSpecLocation[2];
	GLuint uniformDynamicGlowColorLocation[2];
	GLuint uniformStaticGlowIntensityLocation[2];
	GLuint uniformGlowTrail[2];
	
	GLuint uniformDynamicGlowColorLocation_wall;
	
	GLuint uniformGlowTimeDelta;
	GLuint uniformGlowFalloff;
	GLuint uniformGlowTexture;
	GLuint uniformGlowSelf;

	GLuint uniformKeyMatrixLocation;

	//particle shaders
	GLuint particleCS;
	GLuint particleShader;
	GLuint particleSize;
	GLuint particleViewProj;
	GLuint particleCam;
	GLuint particleTexture;

	//lightwall
	GLuint lwVertexDataId;
	GLuint lwVertexAttribute;
	float lwPosDefault[6] = { 0 };

	GLuint lw_Shader;
	GLuint lw_pos1;
	GLuint lw_pos2;
	GLuint lw_uv1;
	GLuint lw_uv2;
	GLuint lw_tex;
	GLuint lw_vp;

	float timepass = 0;
	float delta = 0;

	string result;

	float counter;
	int renderFrameTimeID;

	bool initialized;

	bool renderDebugText = false;

	vector<Text*> textObjects;

	//take damage effect
	float takeDamage_startDispalce;
	float takeDamage_timer;
	float takeDamage_timerStartValue;

	mat4 minimapRenderMat;

	Crosshair* cross;

	GLuint crosshairTexture;

	ParticleSystem particleTest;
	ParticleSystemData pdata;
	GLuint ptex;

public:

	RenderPipeline() {};

	void renderEffects();

	virtual bool init(unsigned int WindowWidth, unsigned int WindowHeight);
	virtual void release();
	virtual void update(float x, float y, float z, float dt);
	virtual void renderIni();

	virtual void reloadShaders();

	virtual void render();
	virtual void finalizeRender();

	virtual void addLight(SpotLight* newLight, int roomID);
	virtual void setChunkColorAndInten(int ID, float* color, float inten);

	virtual void* getView();

	virtual void renderMISC(int miscID, void* world, float* dgColor, float sgInten);
	virtual void renderBullet(int bid, void* world, float* dgColor, float sgInten);
	virtual void renderAnimation(int playerID, int roleID, void* world, AnimationState animState, float* dgColor, float sgInten, bool first, bool primary, int roomID);

	virtual std::string getStatus() { return ""; };

	virtual bool setSetting(PIPELINE_SETTINGS type, PipelineValues value);
	virtual void renderWallEffect(void* pos1, void* pos2, float uvStartOffset, float* dgColor);
	virtual void renderExploEffect(float* pos, float rad, float transp, float* dgColor);
	virtual void renderThunderDomeEffect(float* pos, float rad, float transp, float* dgColor);

	virtual void renderDecals(void* data, int size);

	virtual void renderCrosshair(CROSSHAIR_TYPE cross);

	virtual void renderCapturePoint(int capPointID);

	virtual void forceReset() {};

	virtual void setChatHistoryText(std::string text);
	virtual void setChatTypeMessage(std::string text);

	virtual int createTextObject(std::string text, int fontSize, glm::vec2 pos);
	virtual void setTextObjectText(int id, std::string text);
	virtual void setTextPos(int id, glm::vec2 pos);
	virtual void removeTextObject(int id);
	virtual void renderTextObject(int id);
	virtual void renderTextObjectWorldPos(int id, glm::mat4 world);

	virtual void setCapRoomColor(int capPoint, vec3 color, float intensity);

	virtual void setGBufferWin(unsigned int WindowWidth, unsigned int WindowHeight);

	virtual void* getChunkCollisionVectorAsPoint(int chunkID);
	virtual void* getCapPointsAsPoint(int& count);
	virtual void* getRoomBoxes();

	virtual void getPlayerBox(float &xMax, float &xMin, float &yMax, float &yMin, float &zMax, float &zMin);
	virtual void getWorldBoxes(int &current, float &xMax, float &xMin, float &yMax, float &yMin, float &zMax, float &zMin);
	virtual void getSpawnpoints(std::vector < std::vector < SpawnpointG > > &spoints);
	virtual int getNrOfWorldBoxes();
	virtual void setRenderFlag(RENDER_FLAGS flag);

	virtual void ui_initRender();
	virtual void ui_InGameRenderInit();
	virtual void ui_loadTexture(unsigned int* texid, char* filepath, int* xres, int* yres);
	virtual void ui_renderQuad(float* mat, float* pivot, unsigned int textureID, float transp, int i);
	virtual void ui_textureRelease(vector<unsigned int> texids);
	virtual int portalIntersection(float* pos1, float* pos2, int in_chunk);
	virtual void setCullingCurrentChunkID(int roomID);

	virtual void enableDepthTest();
	virtual void disableDepthTest();
	virtual void enableBlend();
	virtual void disableBlend();
	virtual void clearBothBuffers();
	virtual void clearColor();

	virtual int startExecTimer(std::string name);
	virtual void stopExecTimer(int id);
	virtual bool* getRenderedChunks(int& get_size);

	virtual void startTakeDamageEffect(int maxDisplace, float time);
	virtual void updateTakeDamageEffect(float dt);
	virtual void renderMinimap(float* yourPos, float* yourdir, float* teammates, int nrOfTeammates, int team);
	virtual void renderScoreBoard(int team1size, int team2size);
};


#endif