#ifndef ENGINE_H
#define ENGINE_H

#include "IRenderPipeline.h"
#include "Text\TextObject.h"
#include "Camera.h"
#include "GBuffer.h"

#include "ContentManager.h"
#include "Effects\LightWall.h"

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

	LightWall lw;

	TextObject* test;

	Camera cam;

	Gbuffer* gBuffer;

	GLuint regularShader;

	GLuint worldMat;
	GLuint viewProjMat;
	GLuint viewMat;

	GLuint uniformTextureLocation;
	GLuint uniformNormalLocation;
	GLuint uniformGlowSpecLocation;
	GLuint uniformDynamicGlowColorLocation;
	GLuint uniformStaticGlowIntensityLocation;

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
public:

	RenderPipeline() {};

	void renderEffects();

	virtual bool init(unsigned int WindowWidth, unsigned int WindowHeight);
	virtual void release();
	virtual void update(float x, float y, float z, float dt);
	virtual void renderIni();
	virtual void render();

	virtual void addLight(SpotLight* newLight);
	virtual void setChunkColorAndInten(int ID, float* color, float inten);

	virtual void* getView();

	virtual void renderPlayer(int playerID, void* world, float* dgColor, float sgInten);

	virtual std::string getStatus() { return ""; };

	virtual bool setSetting(PIPELINE_SETTINGS type, PipelineValues value);
	virtual void renderWallEffect(void* pos1, void* pos2, float uvStartOffset);

	virtual void forceReset() {};
	virtual unsigned int createText(float x, float y, float z, std::string text) { return 0; };
	virtual void removeText(unsigned int textID) {};
	virtual void setText(unsigned int id, std::string text) {};
	virtual void setTextPos(unsigned int, float x, float y, float z) {};

	virtual unsigned int createTexture(std::string fileName) { return 0; };
	virtual unsigned int createTextureFromMemory(unsigned int size, void* data) { return 0; };
	virtual void removeTexture(unsigned int) {};

	virtual unsigned int createMesh(...) { return 0; }; // fix parameters
	virtual void removeMesh(unsigned int id) {};

	virtual void setGBufferWin(unsigned int WindowWidth, unsigned int WindowHeight);

	virtual void* getChunkCollisionVectorAsPoint(int chunkID);

	virtual void getPlayerBox(float &xMax, float &xMin, float &yMax, float &yMin, float &zMax, float &zMin);
	virtual void getWorldBoxes(int &current, float &xMax, float &xMin, float &yMax, float &yMin, float &zMax, float &zMin);
	virtual void getSpawnpoints(std::vector < std::vector < SpawnpointG > > &spoints);
	virtual int getNrOfWorldBoxes();
	virtual void setRenderFlag(RENDER_FLAGS flag);

};


#endif