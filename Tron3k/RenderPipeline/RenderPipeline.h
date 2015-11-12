#ifndef ENGINE_H
#define ENGINE_H

#include "IRenderPipeline.h"
#include "Text\TextObject.h"

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

	NONE_IN = -1,
};

class RenderPipeline : public IRenderPipeline
{
private:

	SETTING_INPUT getType(PIPELINE_SETTINGS type) const;

	TextObject* test;

public:

	RenderPipeline() {};

	virtual bool init();
	virtual void release();
	virtual void update();
	virtual void render();

	virtual std::string getStatus() { return ""; };

	virtual bool setSetting(PIPELINE_SETTINGS type, PipelineValues value);

	virtual void forceReset() {};
	virtual unsigned int createText(float x, float y, float z, std::string text) { return 0; };
	virtual void removeText(unsigned int textID) {};
	void setText(unsigned int id, std::string text) {};
	void setTextPos(unsigned int, float x, float y, float z) {};

};


#endif