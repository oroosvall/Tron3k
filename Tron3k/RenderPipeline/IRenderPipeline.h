#ifndef IENGINE_H
#define IENGINE_H

#ifdef RENDERPIPELINE_EXPORTS
#define ENGINE_API __declspec( dllexport)
#else
#define ENGINE_API __declspec( dllimport  )
#endif

#include <string>

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

class IRenderPipeline
{
public:
	virtual bool init(unsigned int WindowWidth, unsigned int WindowHeight) = 0;
	virtual void release() = 0;
	virtual void update() = 0;
	virtual void renderIni() = 0;
	virtual void render() = 0;

	virtual void* getView() = 0;

	virtual void renderPlayer(int playerID, void* world) = 0;

	virtual std::string getStatus() = 0;

	PipelineValues getSettings(PIPELINE_SETTINGS type);
	virtual bool setSetting(PIPELINE_SETTINGS type, PipelineValues value) = 0;

	virtual void forceReset() = 0;
	virtual unsigned int createText(float x, float y, float z, std::string text) = 0;
	virtual void removeText(unsigned int textID) = 0;
	virtual void setText(unsigned int id, std::string text) = 0;
	virtual void setTextPos(unsigned int, float x, float y, float z) = 0;

	virtual unsigned int createTexture(std::string fileName) = 0;
	virtual unsigned int createTextureFromMemory(unsigned int size, void* data) = 0;
	virtual void removeTexture(unsigned int) = 0;

	virtual unsigned int createMesh(...) = 0;
	virtual void removeMesh(unsigned int id) = 0;

	virtual void setGBufferWin(unsigned int WindowWidth, unsigned int WindowHeight) = 0;

	virtual void* getMinExtremes() = 0;
	virtual void* getMaxExtremes() = 0;

};

extern "C" ENGINE_API IRenderPipeline* CreatePipeline();

#endif