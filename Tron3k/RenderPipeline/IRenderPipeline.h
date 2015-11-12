#ifndef IENGINE_H
#define IENGINE_H

#ifdef RENDERPIPELINE_EXPORTS
#define ENGINE_API __declspec( dllexport)
#else
#define ENGINE_API __declspec( dllimport  )
#endif

enum SETTING_TYPE
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

struct SettingsValue
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

	SettingsValue getSettings(SETTING_TYPE type);
	bool setSetting(SETTING_TYPE type, SettingsValue value);

};

extern "C" ENGINE_API IRenderPipeline* CreateEngine();

#endif