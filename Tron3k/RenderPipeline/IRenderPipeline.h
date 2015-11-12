#ifndef IENGINE_H
#define IENGINE_H

#ifdef RENDERPIPELINE_EXPORTS
#define ENGINE_API __declspec( dllexport)
#else
#define ENGINE_API __declspec( dllimport  )
#endif

enum PIPELINE_SETTINGS
{
	CLEAR_COLOR,
	GLOW,
	SHADOWS,
	AO,
	VIEWPORT,
	REDUCEPARTICLES,
	DOF,
	DOF_VALUE
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
	virtual bool init() = 0;
	virtual void release() = 0;
	virtual void update() = 0;
	virtual void render() = 0;

	PipelineValues getSettings(PIPELINE_SETTINGS type);
	virtual bool setSetting(PIPELINE_SETTINGS type, PipelineValues value) = 0;

};

extern "C" ENGINE_API IRenderPipeline* CreatePipeline();

#endif