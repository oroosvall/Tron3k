#include "AnimatedTexture.h"
#include "Manager/TextureManager.h"

AnimatedTexture::AnimatedTexture()
{

}

AnimatedTexture::~AnimatedTexture()
{

}

void AnimatedTexture::init()
{
	if (!initialized)
	{
		water.textureID = TextureManager::gTm->createTexture("GameFiles/Textures/animatedSigns/water.png");
	}
	water.init_ground(vec3(-225.0f, -4.5f, 225.0f), vec3(225.0f, -4.5f, -225.0f));
	water.init_time_segments(1, 0.0f, 0.0f);

	if (!initialized)
	{
		water_specular.textureID = TextureManager::gTm->createTexture("GameFiles/Textures/Map/waterfront_normal.png");
	}
	water_specular.init_ground(vec3(-225.0f, -4.4f, 225.0f), vec3(225.0f, -4.4f, -225.0f));
	water_specular.init_time_segments(1, 0.0f, 0.0f);

	int n = 0;
	if (!initialized)
	{
		objects.push_back(AnimatedObject());
		objects[n].textureID = TextureManager::gTm->createTexture("GameFiles/Textures/animatedSigns/spaceNow.dds");
	}
	objects[n].init_standing_right(vec3(94.8f, 22.3f, 53.6f + 3), vec3(94.8f, 14.3f, 74.3f + 3));
	objects[n].init_time_segments(20, 0.075f, 0);

	n++;
	if (!initialized)
	{
		objects.push_back(AnimatedObject());
		objects[n].textureID = TextureManager::gTm->createTexture("GameFiles/Textures/animatedSigns/youmuSwing.png");
	}
	objects[n].init_standing_right(vec3(45, 2, 103.7f), vec3(41.8f, 0.6f, 103.7f));
	objects[n].init_time_segments(5, 0.15f, 0);
	
	n++;
	if (!initialized)
	{
		objects.push_back(AnimatedObject());
		objects[n].textureID = TextureManager::gTm->createTexture("GameFiles/Textures/animatedSigns/glowStrip.png");
	}
	objects[n].init_standing_down(vec3(94.99f, 50, 49.4f), vec3(94.99f, 3, 49.7f));
	objects[n].init_time_segments(1, 0, 3.0f);

	n++;
	if (!initialized)
	{
		objects.push_back(AnimatedObject());
		objects[n].textureID = objects[n-1].textureID;
	}
	objects[n].init_standing_down(vec3(94.99f, 50, 83.5f), vec3(94.99f, 3, 83.8f));
	objects[n].init_time_segments(1, 0, 3.0f);
	objects[n].timer = 2.0f;

	n++;
	if (!initialized)
	{
		objects.push_back(AnimatedObject());
		objects[n].textureID = objects[n - 1].textureID;
	}
	objects[n].init_standing_down(vec3(49.3f, 55, 80.91f), vec3(49.0f, 0.0f, 80.91f));
	objects[n].init_time_segments(1, 0, 3.0f);
	objects[n].timer = 3.5f;

	n++;
	if (!initialized)
	{
		objects.push_back(AnimatedObject());
		objects[n].textureID = objects[n - 1].textureID;
	}
	objects[n].init_standing_down(vec3(41.0f, 55, 80.91f), vec3(41.3f, 0.0f, 80.91f));
	objects[n].init_time_segments(1, 0, 3.0f);
	objects[n].timer = 4.5f;

	//n++;
	//if (!initialized)
	//{
	//	objects.push_back(AnimatedObject());
	//	objects[n].textureID = objects[n - 1].textureID;
	//}
	//objects[n].init_standing_down(vec3(94.99f, 50, 83.5f), vec3(94.99f, 3, 83.8f));
	//objects[n].init_time_segments(1, 0, 3.0f);
	//objects[n].timer = 2.0f;

	n++;
	if (!initialized)
	{
		objects.push_back(AnimatedObject());
		objects[n].textureID = TextureManager::gTm->createTexture("GameFiles/Textures/animatedSigns/discoWallAnimated.png");
	}
	objects[n].init_standing_right(vec3(40.0f, 4.0f, 94.6f), vec3(46.0f, 1.0f, 94.6f));
	objects[n].init_time_segments(9, 0.4f, 0);

	n++;
	if (!initialized)
	{
		objects.push_back(AnimatedObject());
		objects[n].textureID = TextureManager::gTm->createTexture("GameFiles/Textures/animatedSigns/seasons.png");
	}
	objects[n].init_standing_right(vec3(-69.3f, 12, 57.5f), vec3(-69.3f, 5, 55.5f));
	objects[n].init_time_segments(4, 2.0f, 0.5f);

	n++;
	if (!initialized)
	{
		objects.push_back(AnimatedObject());
		objects[n].textureID = TextureManager::gTm->createTexture("GameFiles/Textures/animatedSigns/discoFloor.png");
	}
	objects[n].init_ground(vec3(39.5f, 0.2f , 96), vec3(46.5f, 0.2f, 102));
	objects[n].init_time_segments(5, 0.6f, 0.0f);


	n++;
	if (!initialized)
	{
		objects.push_back(AnimatedObject());
		objects[n].textureID = TextureManager::gTm->createTexture("GameFiles/Textures/animatedSigns/alphaSign.png");
	}
	objects[n].worldMat[0] = vec4(-5.12f, 8.22f, 113.79f, 1.0f);
	objects[n].worldMat[1] = vec4(-5.12f + 0.5f, 8.22f - 1, 113.79f, 1.0f);
	
	objects[n].worldMat[2] = vec4(-8.90f, 5.67f, 113.68f, 1.0f);
	objects[n].worldMat[3] = vec4(-8.90f + 0.5f, 5.67f - 1, 113.68f, 1.0f);




	objects[n].init_time_segments(5, 0.4f, 0.f);

	n++;
	initialized = true;
}

void AnimatedTexture::update(float dT)
{
	timepass += dT;

	//if (timepass < 0)
	//	timepass = 0;

	int size = objects.size();

	for (int n = 0; n < size; n++)
	{
		objects[n].update(dT);
	}
	water.update(dT);
	water_specular.update(dT);
}

void AnimatedTexture::render()
{
	

	glUseProgram(animQuadShader);
	glDisable(GL_CULL_FACE);
	glActiveTexture(GL_TEXTURE0);

	glDisable(GL_BLEND);
	//water specular render
	glProgramUniform1f(animQuadShader, animQuadTime, timepass);
	glProgramUniform1i(animQuadShader, animQuadType, 1);
	TextureManager::gTm->bindTextureOnly(water_specular.textureID, TEXTURE_FALLBACK::DIFFUSE_FB);
	glProgramUniformMatrix4fv(animQuadShader, animQuadWorld, 1, GL_FALSE, (GLfloat*)&water_specular.worldMat[0][0]);
	glProgramUniform2fv(animQuadShader, animQuadUVset, 1, (GLfloat*)&water_specular.offsets[0]);
	glDrawArrays(GL_POINTS, 0, 1);


	glEnable(GL_BLEND);
	glProgramUniform1f(animQuadShader, animQuadTime, timepass);
	glProgramUniform1i(animQuadShader, animQuadType, 0);
	int size = objects.size();

	for (int n = 0; n < size; n++)
	{
		TextureManager::gTm->bindTextureOnly(objects[n].textureID, TEXTURE_FALLBACK::DIFFUSE_FB);

		glProgramUniformMatrix4fv(animQuadShader, animQuadWorld, 1, GL_FALSE, (GLfloat*)&objects[n].worldMat[0][0]);
		glProgramUniform2fv(animQuadShader, animQuadUVset, 1, (GLfloat*)&objects[n].offsets[0]);
		

		glDrawArrays(GL_POINTS, 0, 1);
	}

	//water render
	glProgramUniform1f(animQuadShader, animQuadTime, timepass);
	glProgramUniform1i(animQuadShader, animQuadType, 2);
	TextureManager::gTm->bindTextureOnly(water.textureID, TEXTURE_FALLBACK::DIFFUSE_FB);
	glProgramUniformMatrix4fv(animQuadShader, animQuadWorld, 1, GL_FALSE, (GLfloat*)&water.worldMat[0][0]);
	glProgramUniform2fv(animQuadShader, animQuadUVset, 1, (GLfloat*)&water.offsets[0]);

	glDrawArrays(GL_POINTS, 0, 1);
}