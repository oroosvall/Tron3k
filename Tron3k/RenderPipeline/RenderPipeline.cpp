#include "RenderPipeline.h"
#include <iostream>
#include <GL\glew.h>

#include "Shader.h"
#include "Utils\GPUMemoryLeakChecker.h"

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

	worldMat = glGetUniformLocation(regularShader, "WorldMatrix"); //worldMat
	viewMat = glGetUniformLocation(regularShader, "ViewMatrix"); //view
	viewProjMat = glGetUniformLocation(regularShader, "ViewProjMatrix"); //view

	uniformTextureLocation = glGetUniformLocation(regularShader, "textureSample"); //view
	uniformNormalLocation = glGetUniformLocation(regularShader, "normalSample"); //view
	uniformGlowSpecLocation = glGetUniformLocation(regularShader, "glowSpecSample"); //view

	cam.setViewMat(regularShader, viewMat);
	worldMat = glGetUniformLocation(regularShader, "WorldMatrix"); //worldMat
	viewMat = glGetUniformLocation(regularShader, "ViewProjMatrix"); //view

	uniformTextureLocation = glGetUniformLocation(regularShader, "textureSample"); //view

	uniformDynamicGlowColorLocation = glGetUniformLocation(regularShader, "dynamicGlowColor");
	uniformStaticGlowIntensityLocation = glGetUniformLocation(regularShader, "staticGlowIntensity");

	cam.setViewProjMat(regularShader, viewProjMat);
	cam.setViewMat(regularShader, viewMat);
	
	gBuffer->init(WindowWidth, WindowHeight, 5, true);

	contMan.init();
	
	//light wall init INIT 2 points then change all info though uniforms to build quads
	glGenBuffers(1, &lwVertexDataId);
	glBindBuffer(GL_ARRAY_BUFFER, lwVertexDataId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * 2 , &lwPosDefault[0], GL_STATIC_DRAW);
	glGenVertexArrays(1, &lwVertexAttribute);
	glBindVertexArray(lwVertexAttribute);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, BUFFER_OFFSET(0));
	//build shader
	std::string shaderNamesLW[] = { "GameFiles/Shaders/lw_shader_vs.glsl", "GameFiles/Shaders/lw_shader_gs.glsl", "GameFiles/Shaders/lw_shader_fs.glsl" };
	GLenum shaderTypesLW[] = { GL_VERTEX_SHADER, GL_GEOMETRY_SHADER, GL_FRAGMENT_SHADER };
	CreateProgram(lw_Shader, shaderNamesLW, shaderTypesLW, 3);
	//get uniform locations
	lw_pos1 = glGetUniformLocation(lw_Shader, "pos1");
	lw_pos2	= glGetUniformLocation(lw_Shader, "pos2");
	lw_uv1	= glGetUniformLocation(lw_Shader, "uv1");
	lw_uv2	= glGetUniformLocation(lw_Shader, "uv2");
	lw_vp	= glGetUniformLocation(lw_Shader, "ViewProjMatrix");
	lw_tex	= glGetUniformLocation(lw_Shader, "texsample");

	return true;
}

void RenderPipeline::release()
{
	// place delete code here

	glDeleteBuffers(1, &lwVertexDataId);
	glDeleteVertexArrays(1, &lwVertexAttribute);

	glDeleteShader(lw_Shader);
	glDeleteShader(regularShader);
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	delete test;

	delete gBuffer;

	contMan.release();

	reportGPULeaks();

	delete this; // yes this is safe
}

void RenderPipeline::update(float x, float y, float z, float dt)
{
	timepass += dt;
	//set camera matrixes
	cam.setViewMat(regularShader, viewMat);
	cam.setViewProjMat(regularShader, viewProjMat);
	cam.setViewProjMat(*gBuffer->portal_shaderPtr, gBuffer->portal_vp);

	gBuffer->eyePosLast = gBuffer->eyePos;
	gBuffer->eyePos.x = x;
	gBuffer->eyePos.y = y;
	gBuffer->eyePos.z = z;

	//returns room id, new value if we went though a portal
	contMan.getPortalID(gBuffer->eyePosLast, gBuffer->eyePos);

	glProgramUniform1i(regularShader, uniformTextureLocation, 0);
	glProgramUniform1i(regularShader, uniformNormalLocation, 1);
	glProgramUniform1i(regularShader, uniformGlowSpecLocation, 2);

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
	contMan.renderChunks(regularShader, worldMat, uniformTextureLocation, uniformNormalLocation, uniformGlowSpecLocation, uniformDynamicGlowColorLocation, uniformStaticGlowIntensityLocation,  *gBuffer->portal_shaderPtr, gBuffer->portal_model);
	
	//glDepthMask(GL_TRUE);glEnable(GL_CULL_FACE);glDisable(GL_BLEND);)
	renderEffects();

	glDepthMask(GL_TRUE);
	glEnable(GL_CULL_FACE);
	glDisable(GL_BLEND);

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

void RenderPipeline::renderWallEffect(void* pos1, void* pos2, float uvStartOffset)
{
	//glDepthMask(GL_FALSE);
	glDisable(GL_CULL_FACE);
	//glEnable(GL_BLEND);

	glUseProgram(lw_Shader);
	glProgramUniform1i(lw_Shader, lw_tex, 0);
	//call contentman and bind the lightwal texture to 0
	contMan.bindLightwalTexture();
	cam.setViewProjMat(lw_Shader, lw_vp);

	glBindBuffer(GL_ARRAY_BUFFER, lwVertexDataId);
	glBindVertexArray(lwVertexAttribute);

	float of = timepass * 0.1f;

	glm::vec3 wpos1 = *(glm::vec3*)pos1;
	glm::vec3 wpos2 = *(glm::vec3*)pos2;

	float dist = glm::distance(wpos1, wpos2);

	vec2 uv1 = vec2(uvStartOffset + of, 0);
	vec2 uv2 = vec2(uvStartOffset + dist + of, 0);
	glProgramUniform2fv(lw_Shader, lw_uv1, 1, &uv1[0]);
	glProgramUniform2fv(lw_Shader, lw_uv2, 1, &uv2[0]);
	glProgramUniform3fv(lw_Shader, lw_pos1, 1, &wpos1[0]);
	glProgramUniform3fv(lw_Shader, lw_pos2, 1, &wpos2[0]);

	glDrawArrays(GL_POINTS, 0, 2);

	glEnable(GL_CULL_FACE);
	//glDisable(GL_BLEND);
	//glDepthMask(GL_TRUE);

}

void RenderPipeline::renderEffects()
{
	glUseProgram(lw_Shader);
	glProgramUniform1i(lw_Shader, lw_tex, 0);
	//call contentman and bind the lightwal texture to 0
	contMan.bindLightwalTexture();
	cam.setViewProjMat(lw_Shader, lw_vp);

	glBindBuffer(GL_ARRAY_BUFFER, lwVertexDataId);
	glBindVertexArray(lwVertexAttribute);
	
	float of = timepass * 0.1f;

	for (size_t i = 0; i < lw.playerWall.size(); i++)
	{
		for (size_t w = 0; w < lw.playerWall[i].lightQuad.size(); w++)
		{
			vec2 uv1 = lw.playerWall[i].lightQuad[w].uv1 + vec2(of, 0);
			vec2 uv2 = lw.playerWall[i].lightQuad[w].uv2 + vec2(of, 0);
			glProgramUniform2fv(lw_Shader, lw_uv1, 1, &uv1[0]);
			glProgramUniform2fv(lw_Shader, lw_uv2, 1, &uv2[0]);
			glProgramUniform3fv(lw_Shader, lw_pos1, 1, &lw.playerWall[i].lightQuad[w].pos1[0]);
			glProgramUniform3fv(lw_Shader, lw_pos2, 1, &lw.playerWall[i].lightQuad[w].pos2[0]);

			glDrawArrays(GL_POINTS, 0, 2);
		}
	}

	//vec2 uv1 = vec2(of, 0);
	//vec2 uv2 = vec2(3 + of, 0);
	//glProgramUniform2fv(lw_Shader, lw_uv1, 1, &uv1[0]);
	//glProgramUniform2fv(lw_Shader, lw_uv2, 1, &uv2[0]);
	//vec3 pos1 = vec3(0, 3, 0);
	//vec3 pos2 = vec3(0, 3, 6);
	//glProgramUniform3fv(lw_Shader, lw_pos1, 1, &pos1[0]);
	//glProgramUniform3fv(lw_Shader, lw_pos2, 1, &pos2[0]);
	//
	//glDrawArrays(GL_POINTS, 0, 2);
	
	//uv1 = vec2(3 + of, 0);
	//uv2 = vec2(5 + of, 0);
	//glProgramUniform2fv(lw_Shader, lw_uv1, 1, &uv1[0]);
	//glProgramUniform2fv(lw_Shader, lw_uv2, 1, &uv2[0]);
	////pos1 = vec3(0, 3, 6);
	////pos2 = vec3(0, 5, 8);
	//glProgramUniform3fv(lw_Shader, lw_pos1, 1, &pos1[0]);
	//glProgramUniform3fv(lw_Shader, lw_pos2, 1, &pos2[0]);
	//
	//glDrawArrays(GL_POINTS, 0, 2);
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
	//Glow values for player
	glProgramUniform1f(regularShader, uniformStaticGlowIntensityLocation, sgInten);
	glProgramUniform3fv(regularShader, uniformDynamicGlowColorLocation, 1, (GLfloat*)&dgColor[0]);

	//set temp objects worldmat
	glProgramUniformMatrix4fv(regularShader, worldMat, 1, GL_FALSE, (GLfloat*)world);

	contMan.renderPlayer(playerID, *(glm::mat4*)world);
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

void RenderPipeline::setRenderFlag(RENDER_FLAGS flag)
{
	switch (flag)
	{
		case PORTAL_CULLING:	contMan.f_portal_culling = !contMan.f_portal_culling;	break;
		case FREEZE_CULLING:	contMan.f_freeze_portals = !contMan.f_freeze_portals;	break;
		case RENDER_CHUNK:		contMan.f_render_chunks = !contMan.f_render_chunks;		break;
		case RENDER_ABB:		contMan.f_render_abb = !contMan.f_render_abb;			break;
		case RENDER_OBB:		contMan.f_render_obb = !contMan.f_render_obb;			break;
	}
}

void RenderPipeline::getSpawnpoints(std::vector < std::vector < SpawnpointG > > &spoints)
{
	//add teams
	vector<SpawnpointG> team0;
	vector<SpawnpointG> team1;
	vector<SpawnpointG> team2;

	spoints.push_back(team0);
	spoints.push_back(team1);
	spoints.push_back(team2);
}