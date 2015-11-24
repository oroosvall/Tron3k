#ifndef ENGINE_H
#define ENGINE_H

#include "IRenderPipeline.h"
#include "Text\TextObject.h"
#include "Camera.h"
#include "SimpleShit.h"
#include "GBuffer.h"

#include "ContentManager.h"

//#include <vld.h>

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

	TextObject* test;

	Camera cam;

	Gbuffer* gBuffer;

	GLuint regularShader;

	GLuint worldMat;
	GLuint viewProjMat;
	GLuint viewMat;

	GLuint uniformTextureLocation;
	GLuint uniformnNormalLocation;

	//temp
	TestMesh testMesh;


public:

	RenderPipeline() {};

	virtual bool init(unsigned int WindowWidth, unsigned int WindowHeight);
	virtual void release();
	virtual void update(float x, float y, float z, float dt);
	virtual void renderIni();
	virtual void render();

	virtual void* getView();

	virtual void renderPlayer(int playerID, void* world);

	virtual std::string getStatus() { return ""; };

	virtual bool setSetting(PIPELINE_SETTINGS type, PipelineValues value);

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
	virtual void* getMinExtremes();
	virtual void* getMaxExtremes();

};


#endif