#include "RenderPipeline.h"
#include <iostream>
#include <GL\glew.h>

#include "Shader.h"

#ifdef _DEBUG
extern "C"
{
	void __stdcall openglCallbackFunction(GLenum source,
		GLenum type,
		GLuint id,
		GLenum severity,
		GLsizei length,
		const GLchar* message,
		void* userParam)
	{
		if (severity == GL_DEBUG_SEVERITY_LOW || severity == GL_DEBUG_SEVERITY_MEDIUM || severity == GL_DEBUG_SEVERITY_HIGH)
		{
			printf("---------------------opengl-callback-start------------\n");
			printf("message: %s\n", message);
			printf("type: ");
			switch (type) {
			case GL_DEBUG_TYPE_ERROR:
				printf("ERROR");
				break;
			case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
				printf("DEPRECATED_BEHAVIOR");
				break;
			case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
				printf("UNDEFINED_BEHAVIOR");
				break;
			case GL_DEBUG_TYPE_PORTABILITY:
				printf("PORTABILITY");
				break;
			case GL_DEBUG_TYPE_PERFORMANCE:
				printf("PERFORMANCE");
				break;
			case GL_DEBUG_TYPE_OTHER:
				printf("OTHER");
				break;
			}
			printf("\n");

			printf("id: %d\n", id);
			printf("severity: ");
			switch (severity) {
			case GL_DEBUG_SEVERITY_LOW:
				printf("LOW");
				break;
			case GL_DEBUG_SEVERITY_MEDIUM:
				printf("MEDIUM");
				break;
			case GL_DEBUG_SEVERITY_HIGH:
				printf("HIGH");
				break;
			}
			printf("\n");
			printf("---------------------opengl-callback-end--------------\n");
		}
	}

}
#endif

bool RenderPipeline::init()
{
	GLenum result = glewInit();
	if (result != GLEW_OK)
	{
		return false;
	}

	cam.init();

	test = new TextObject("Swag", 11, glm::vec2(10, 10));


#ifdef _DEBUG
	if (glDebugMessageCallback) {
		printf("Register OpenGL debug callback\n");
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback((GLDEBUGPROC)openglCallbackFunction, nullptr);
		GLuint unusedIds = 0;
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, &unusedIds, true);
	}
	else
		printf("glDebugMessageCallback not available\n");
#endif

	std::string shaderNames[] = { "GameFiles/Shaders/simple_vs.glsl", "GameFiles/Shaders/simple_fs.glsl" };
	GLenum shaderTypes[] = { GL_VERTEX_SHADER, GL_FRAGMENT_SHADER };

	CreateProgram(testShader, shaderNames, shaderTypes, 2);

	worldMat = glGetUniformLocation(testShader, "worldMat");
	projMat = glGetUniformLocation(testShader, "project");
	viewMat = glGetUniformLocation(testShader, "view");

	cam.setProjMat(testShader, projMat);
	cam.setViewMat(testShader, viewMat);

	testMesh.make();

	return true;
}

void RenderPipeline::release()
{
	// place delete code here

	delete test;

	delete this; // yes this is safe
}

void RenderPipeline::update()
{

}

void RenderPipeline::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//glProgramUniformMatrix4fv(testShader, worldMat, 1, GL_FALSE, &); // todo fix..

	glBindBuffer(GL_ARRAY_BUFFER, testMesh.vbuffer);
	glBindVertexArray(testMesh.vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, testMesh.index);

	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
	


}

bool RenderPipeline::setSetting(PIPELINE_SETTINGS type, PipelineValues value)
{
	if (value.type != getType(type))
	{
		return false;
	}
	else
	{

	}
	return true;
}

SETTING_INPUT RenderPipeline::getType(PIPELINE_SETTINGS type) const
{

	switch (type) {
	case PIPELINE_SETTINGS::CLEAR_COLOR:
		return SETTING_INPUT::CLEAR_COLOR_IN;
	case PIPELINE_SETTINGS::GLOW:
		return SETTING_INPUT::GLOW_IN;
	case PIPELINE_SETTINGS::SHADOWS:
		return SETTING_INPUT::SHADOWS_IN;
	case PIPELINE_SETTINGS::AO:
		return SETTING_INPUT::AO_IN;
	case PIPELINE_SETTINGS::VIEWPORT:
		return SETTING_INPUT::VIEWPORT_IN;
	case PIPELINE_SETTINGS::REDUCEPARTICLES:
		return SETTING_INPUT::REDUCEPARTICLES_IN;
	case PIPELINE_SETTINGS::DOF:
		return SETTING_INPUT::DOF_IN;
	case PIPELINE_SETTINGS::DOF_VALUE:
		return SETTING_INPUT::DOF_VALUE_IN;
	default:
		break;
	}

	return SETTING_INPUT::NONE_IN;
}


IRenderPipeline* CreatePipeline()
{
	return new RenderPipeline();
}