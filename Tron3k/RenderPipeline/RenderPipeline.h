#ifndef ENGINE_H
#define ENGINE_H

#include "IRenderPipeline.h"
#include "Text\Text.h"
#include "Camera.h"
#include "Buffer\GBuffer.h"

#include "Manager\ContentManager.h"

#include "Manager\AnimManager.h"

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

	Text* uglyCrosshairSolution;

	GLuint fontTexture;

	string chatHistoryText;
	string chatTypeText;

	GLuint textShader;
	GLuint textShaderLocation;

	Camera cam;

	Gbuffer* gBuffer;

	GLuint regularShader;
	GLuint animationShader;
	GLuint glowShaderTweeks;

	GLuint uiShader;
	GLuint ui_Texture;
	GLuint ui_World;

	BlitQuad uiQuad;

	GLuint worldMat[2];
	GLuint viewProjMat[2];
	GLuint viewMat;

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

	bool initialized;

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

	virtual void addLight(SpotLight* newLight);
	virtual void setChunkColorAndInten(int ID, float* color, float inten);

	virtual void* getView();

	virtual void renderMISC(int miscID, void* world, float* dgColor, float sgInten);
	virtual void renderBullet(int bid, void* world, float* dgColor, float sgInten);
	virtual void renderAnimation(int playerID, int roleID, void* world, AnimationState animState, float* dgColor, float sgInten, bool first);

	virtual std::string getStatus() { return ""; };

	virtual bool setSetting(PIPELINE_SETTINGS type, PipelineValues value);
	virtual void renderWallEffect(void* pos1, void* pos2, float uvStartOffset, float* dgColor);

	virtual void forceReset() {};

	virtual void setChatHistoryText(std::string text);
	virtual void setChatTypeMessage(std::string text);

	virtual unsigned int createMesh(...) { return 0; }; // fix parameters
	virtual void removeMesh(unsigned int id) {};

	virtual void setGBufferWin(unsigned int WindowWidth, unsigned int WindowHeight);

	virtual void* getChunkCollisionVectorAsPoint(int chunkID);

	virtual void getPlayerBox(float &xMax, float &xMin, float &yMax, float &yMin, float &zMax, float &zMin);
	virtual void getWorldBoxes(int &current, float &xMax, float &xMin, float &yMax, float &yMin, float &zMax, float &zMin);
	virtual void getSpawnpoints(std::vector < std::vector < SpawnpointG > > &spoints);
	virtual int getNrOfWorldBoxes();
	virtual void setRenderFlag(RENDER_FLAGS flag);

	virtual void ui_initRender();
	virtual void ui_loadTexture(unsigned int* texid, char* filepath, int* xres, int* yres);
	virtual void ui_renderQuad(float* mat, unsigned int textureID, float transp, int i);
	virtual void ui_textureRelease(vector<unsigned int> texids);
	virtual int portalIntersection(float* pos1, float* pos2, int in_chunk);
	virtual void setCullingCurrentChunkID(int roomID);

};


#endif