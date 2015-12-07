#include "RenderPipeline.h"
#include <iostream>
#include <GL\glew.h>

#include "Shader.h"

//#include "Map\MapHeaders.h"

//#include <vld.h>

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

bool RenderPipeline::init(unsigned int WindowWidth, unsigned int WindowHeight)
{
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	GLenum result = glewInit();
	if (result != GLEW_OK)
	{
		return false;
	}

	cam.init();

	gBuffer = new Gbuffer();
	

	//test = new TextObject("Swag", 11, glm::vec2(10, 10));

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

	glClearColor(0.4f, 0.4f, 0.4f, 1);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_TRUE);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_CULL_FACE); 

	gBuffer->shaderPtr = new GLuint();
	std::string shaderNamesDeffered[] = { "GameFiles/Shaders/BlitLightShader_vs.glsl", "GameFiles/Shaders/BlitLightShader_fs.glsl" };
	GLenum shaderTypesDeffered[] = { GL_VERTEX_SHADER, GL_FRAGMENT_SHADER };
	CreateProgram(*gBuffer->shaderPtr, shaderNamesDeffered, shaderTypesDeffered, 2);

	//portal shader
	gBuffer->portal_shaderPtr = new GLuint();
	std::string shaderPortalRender[] = { "GameFiles/Shaders/portal_vs.glsl", "GameFiles/Shaders/portal_fs.glsl" };
	GLenum shaderTypesPortal[] = { GL_VERTEX_SHADER, GL_FRAGMENT_SHADER };
	CreateProgram(*gBuffer->portal_shaderPtr, shaderPortalRender, shaderTypesPortal, 2);


	std::string shaderNamesRegular[] = { "GameFiles/Shaders/RegularShader_vs.glsl", "GameFiles/Shaders/RegularShader_gs.glsl", "GameFiles/Shaders/RegularShader_fs.glsl" };
	GLenum shaderTypesRegular[] = { GL_VERTEX_SHADER, GL_GEOMETRY_SHADER, GL_FRAGMENT_SHADER };
	CreateProgram(regularShader, shaderNamesRegular, shaderTypesRegular, 3);

	//Skeleton Animation Shader
	std::string shaderNamesAnimation[] = { "GameFiles/Shaders/SkeletonAnimation_vs.glsl", "GameFiles/Shaders/SkeletonAnimation_fs.glsl" };
	GLenum shaderTypesAnimation[] = { GL_VERTEX_SHADER, GL_FRAGMENT_SHADER };
	CreateProgram(animationShader, shaderNamesAnimation, shaderTypesAnimation, 2);

	worldMat[0] = glGetUniformLocation(regularShader, "WorldMatrix"); //worldMat regular shader
	viewMat = glGetUniformLocation(regularShader, "ViewMatrix"); //view
	viewProjMat[0] = glGetUniformLocation(regularShader, "ViewProjMatrix"); //view regular shader

	worldMat[1] = glGetUniformLocation(animationShader, "WorldMatrix"); //worldMat animation shader
	viewProjMat[1] = glGetUniformLocation(animationShader, "ViewProjMatrix"); //view animation shader

	uniformTextureLocation[0] = glGetUniformLocation(regularShader, "textureSample"); //view regular shader
	uniformNormalLocation[0] = glGetUniformLocation(regularShader, "normalSample"); //view regular shader
	uniformGlowSpecLocation[0] = glGetUniformLocation(regularShader, "glowSpecSample"); //view regular shader

	uniformTextureLocation[1] = glGetUniformLocation(animationShader, "textureSample"); //view animation shader
	uniformNormalLocation[1] = glGetUniformLocation(animationShader, "normalSample"); //view animation shader
	uniformGlowSpecLocation[1] = glGetUniformLocation(animationShader, "glowSpecSample"); //view animation shader

	cam.setViewMat(regularShader, viewMat);
	worldMat[0] = glGetUniformLocation(regularShader, "WorldMatrix"); //worldMat regular shader
	viewMat = glGetUniformLocation(regularShader, "ViewProjMatrix"); //view

	worldMat[1] = glGetUniformLocation(animationShader, "WorldMatrix"); //worldMat animation shader

	uniformTextureLocation[0] = glGetUniformLocation(regularShader, "textureSample"); //view regular shader
	uniformTextureLocation[1] = glGetUniformLocation(animationShader, "textureSample"); //view animation shader

	uniformDynamicGlowColorLocation[0] = glGetUniformLocation(regularShader, "dynamicGlowColor"); //regular shader
	uniformStaticGlowIntensityLocation[0] = glGetUniformLocation(regularShader, "staticGlowIntensity"); //regular shader

	uniformDynamicGlowColorLocation[1] = glGetUniformLocation(animationShader, "dynamicGlowColor"); //animation shader
	uniformStaticGlowIntensityLocation[1] = glGetUniformLocation(animationShader, "staticGlowIntensity"); //animation shader

	uniformKeyMatrixLocation = glGetUniformBlockIndex(animationShader, "boneMatrices");

	cam.setViewProjMat(regularShader, viewProjMat[0]);
	cam.setViewProjMat(animationShader, viewProjMat[1]);
	cam.setViewMat(regularShader, viewMat);

	gBuffer->init(WindowWidth, WindowHeight, 5, true);

	contMan.init();

	//void* test = contMan.getChunkCollisionVectorAsPointer(1);

	//vector<ChunkCollision>* test2 = (vector<ChunkCollision>*)test;

	return true;
}

void RenderPipeline::release()
{
	// place delete code here

	glDeleteShader(regularShader);
	glDeleteShader(animationShader);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	delete test;

	delete gBuffer;

	delete this; // yes this is safe
}

void RenderPipeline::update(float x, float y, float z, float dt)
{
	timepass += dt;
	//set camera matrixes
	cam.setViewMat(regularShader, viewMat);
	cam.setViewProjMat(regularShader, viewProjMat[0]);

	cam.setViewProjMat(animationShader, viewProjMat[1]);
	cam.setViewProjMat(*gBuffer->portal_shaderPtr, gBuffer->portal_vp);

	gBuffer->eyePosLast = gBuffer->eyePos;
	gBuffer->eyePos.x = x;
	gBuffer->eyePos.y = y;
	gBuffer->eyePos.z = z;

	//returns room id, new value if we went though a portal
	contMan.getPortalID(gBuffer->eyePosLast, gBuffer->eyePos);

	gBuffer->clearLights();
}
void RenderPipeline::addLight(SpotLight* newLight)
{
	gBuffer->pushLights(newLight, 1);
}


void RenderPipeline::renderIni()
{
	glUseProgram(regularShader);
	gBuffer->bind(GL_FRAMEBUFFER);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RenderPipeline::render()
{
	contMan.renderChunks(regularShader, worldMat[0], uniformTextureLocation[0], uniformNormalLocation[0], uniformGlowSpecLocation[0], uniformDynamicGlowColorLocation[0], uniformStaticGlowIntensityLocation[0],  *gBuffer->portal_shaderPtr, gBuffer->portal_model);

	//push the lights of the rendered chunks
	for (int n = 0; n < contMan.nrChunks; n++)
		if (contMan.renderedChunks[n] == true)
			for (size_t k = 0; k < contMan.testMap.chunks[n].lights.size(); k++)
			{
				gBuffer->pushLights(&contMan.testMap.chunks[n].lights[k], 1);
			}

	//GBuffer Render
	glBindFramebuffer(GL_FRAMEBUFFER, NULL);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	gBuffer->render();
}

void* RenderPipeline::getView()
{
	return (void*)cam.getViewMat();
}

void RenderPipeline::setChunkColorAndInten(int ID, float* color, float inten)
{
	contMan.testMap.chunks[ID].color = { color[0], color[1], color[2] };
	contMan.testMap.chunks[ID].staticIntes = inten;
}

void RenderPipeline::renderPlayer(int playerID, void* world, float* dgColor, float sgInten)
{
	glUseProgram(regularShader);

	//Glow values for player
	glProgramUniform1f(regularShader, uniformStaticGlowIntensityLocation[0], sgInten);
	glProgramUniform3fv(regularShader, uniformDynamicGlowColorLocation[0], 1, (GLfloat*)&dgColor[0]);

	//set temp objects worldmat
	glProgramUniformMatrix4fv(regularShader, worldMat[0], 1, GL_FALSE, (GLfloat*)world);
	
	//set temp objects worldmat
	glProgramUniformMatrix4fv(regularShader, worldMat[0], 1, GL_FALSE, (GLfloat*)world);

	contMan.renderPlayer(playerID, *(glm::mat4*)world, uniformKeyMatrixLocation);
}

void RenderPipeline::renderAnimation(int playerID, void* world, float* dgColor, float sgInten)
{
	glUseProgram(animationShader);

	//Glow values for player
	glProgramUniform1f(animationShader, uniformStaticGlowIntensityLocation[1], sgInten);
	glProgramUniform3fv(animationShader, uniformDynamicGlowColorLocation[1], 1, (GLfloat*)&dgColor[0]);

	//Texture for the glow
	glProgramUniform1i(animationShader, uniformTextureLocation[1], 0);
	glProgramUniform1i(animationShader, uniformNormalLocation[1], 1);
	glProgramUniform1i(animationShader, uniformGlowSpecLocation[1], 2);

	//set temp objects worldmat
	glProgramUniformMatrix4fv(animationShader, worldMat[1], 1, GL_FALSE, (GLfloat*)world);

	contMan.renderPlayer(playerID, *(glm::mat4*)world, uniformKeyMatrixLocation);
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

void* RenderPipeline::getChunkCollisionVectorAsPoint(int chunkID)
{
	return contMan.getChunkCollisionVectorAsPointer(chunkID);
}

void RenderPipeline::getPlayerBox(float &xMax, float &xMin, float &yMax, float &yMin, float &zMax, float &zMin)
{
	//Will be changed later, this is number for now
	xMax = 0.5f;
	xMin = -0.5f;
	yMax = 1.0f;
	yMin = -1.0f;
	zMax = 0.5f;
	zMin = -0.5f;
}

void RenderPipeline::getWorldBoxes(int &current, float &xMax, float &xMin, float &yMax, float &yMin, float &zMax, float &zMin)
{
	std::vector<std::vector<float>> wBoxes = contMan.getMeshBoxes();
	xMax = wBoxes[current][0];
	xMin = wBoxes[current][1];
	yMax = wBoxes[current][2];
	yMin = wBoxes[current][3];
	zMax = wBoxes[current][4];
	zMin = wBoxes[current][5];
}

int RenderPipeline::getNrOfWorldBoxes()
{
	return contMan.getMeshBoxes().size();
}

IRenderPipeline* CreatePipeline()
{
	return new RenderPipeline();
}

void RenderPipeline::setGBufferWin(unsigned int WindowWidth, unsigned int WindowHeight)
{
	gBuffer->resize(WindowWidth, WindowHeight);
}