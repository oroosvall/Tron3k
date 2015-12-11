#include "RenderPipeline.h"
#include <iostream>
#include <GL\glew.h>

#include "Shader.h"
#include "Utils\GPUMemoryLeakChecker.h"

#include <vld.h>

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
	if (initialized)
	{
		return true;
	}

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

	initialized = true;
	return true;
}

void RenderPipeline::release()
{
	// place delete code here

	glDeleteBuffers(1, &lwVertexDataId);
	glDeleteVertexArrays(1, &lwVertexAttribute);

	glDeleteShader(lw_Shader);
	glDeleteShader(regularShader);
	glDeleteShader(animationShader);

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
	
	//glDepthMask(GL_TRUE);glEnable(GL_CULL_FACE);glDisable(GL_BLEND);)
	//renderEffects();

}

void RenderPipeline::finalizeRender()
{

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

	float dist = glm::distance(wpos1, wpos2) / 2.0f;

	vec2 uv1 = vec2(uvStartOffset + of, 0);
	vec2 uv2 = vec2(uvStartOffset + dist + of, 0);
	glProgramUniform2fv(lw_Shader, lw_uv1, 1, &uv1[0]);
	glProgramUniform2fv(lw_Shader, lw_uv2, 1, &uv2[0]);
	glProgramUniform3fv(lw_Shader, lw_pos1, 1, &wpos1[0]);
	glProgramUniform3fv(lw_Shader, lw_pos2, 1, &wpos2[0]);

	glDrawArrays(GL_POINTS, 0, 2);

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
	yMax = 0.3f;  //distance from camera to head max
	yMin = -1.55f; //distance from camera to ground
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

	float yincrease = 7.0f;

	int size = contMan.testMap.spFFACount;
	for (int n = 0; n < size; n++)
	{
		glm::mat4 mat = contMan.testMap.spFFA[n].transform;
		vec3 pos;
		pos.x = mat[0].w;
		pos.y = mat[1].w + yincrease;
		pos.z = mat[2].w;
		team0.push_back(SpawnpointG());
		team0[n].pos = pos;
		team0[n].dir = vec3(contMan.testMap.spFFA[n].dx, contMan.testMap.spFFA[n].dy, contMan.testMap.spFFA[n].dz);
		team0[n].roomID = contMan.testMap.spFFA[n].roomID;
	}
	spoints.push_back(team0);

	size = contMan.testMap.spTACount;
	for (int n = 0; n < size; n++)
	{
		glm::mat4 mat = contMan.testMap.spA[n].transform;
		vec3 pos;
		pos.x = mat[0].w;
		pos.y = mat[1].w + yincrease;
		pos.z = mat[2].w;
		team1.push_back(SpawnpointG());
		team1[n].pos = pos;
		team1[n].dir = vec3(contMan.testMap.spA[n].dx, contMan.testMap.spA[n].dy, contMan.testMap.spA[n].dz);
		team1[n].roomID = contMan.testMap.spA[n].roomID;
	}
	spoints.push_back(team1);

	size = contMan.testMap.spTBCount;
	for (int n = 0; n < size; n++)
	{
		glm::mat4 mat = contMan.testMap.spB[n].transform;
		vec3 pos;
		pos.x = mat[0].w;
		pos.y = mat[1].w + yincrease;
		pos.z = mat[2].w;
		team2.push_back(SpawnpointG());
		team2[n].pos = pos;
		team2[n].dir = vec3(contMan.testMap.spB[n].dx, contMan.testMap.spB[n].dy, contMan.testMap.spB[n].dz);
		team2[n].roomID = contMan.testMap.spB[n].roomID;
	}
	spoints.push_back(team2);

	contMan.testMap.deleteSpawnposData();
}