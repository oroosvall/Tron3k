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
	
	int n = 0;
	if (!initialized)
	{
		objects.push_back(AnimatedObject());
		objects[n].textureID = TextureManager::gTm->createTexture("GameFiles/Textures/animatedSigns/youmuSwing.png");
	}
	objects[n].init_standing_right(vec3(94.8f, 22.3f, 53.6f), vec3(94.8f, 14.3f, 64.3f));
	objects[n].init_time_segments(5, 0.15f, 0);
	
	n++;
	if (!initialized)
	{
		objects.push_back(AnimatedObject());
		objects[n].textureID = TextureManager::gTm->createTexture("GameFiles/Textures/animatedSigns/matrxtext.png");
	}
	objects[n].init_standing_down(vec3(94.8f, 22.3f + 20, 53.6f), vec3(94.8f, 14.3f + 10, 59.3f));
	objects[n].init_time_segments(4, 0, 2.0f);

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
	objects[n].init_standing_right(vec3(94.8f, 22.3f + 20, 60.6f), vec3(94.8f, 14.3f + 10, 64.3f));
	objects[n].init_time_segments(4, 2.0f, 0.5f);

	n++;
	if (!initialized)
	{
		objects.push_back(AnimatedObject());
		objects[n].textureID = TextureManager::gTm->createTexture("GameFiles/Textures/animatedSigns/water.png");
	}
	objects[n].worldMat[0] = vec4( -50.0f, 4.0f, 53.0f, 1 );
	objects[n].worldMat[1] = vec4( -50.0f, 4.0f, 65.0f, 1 );
	objects[n].worldMat[2] = vec4( -50.0f , 8.0f , 58.0f , 1 );
	objects[n].worldMat[3] = vec4( -50.0f, 8.0f, 60.0f, 1 );
	objects[n].init_time_segments(1, 0.0f, 1.0f);

	initialized = true;
}

void AnimatedTexture::update(float dT)
{
	int size = objects.size();

	for (int n = 0; n < size; n++)
	{
		objects[n].update(dT);
	}
}

void AnimatedTexture::render()
{
	glUseProgram(animQuadShader);
	glDisable(GL_CULL_FACE);
	glActiveTexture(GL_TEXTURE0);

	int size = objects.size();

	for (int n = 0; n < size; n++)
	{
		TextureManager::gTm->bindTextureOnly(objects[n].textureID, TEXTURE_FALLBACK::DIFFUSE_FB);

		glProgramUniformMatrix4fv(animQuadShader, animQuadWorld, 1, GL_FALSE, (GLfloat*)&objects[n].worldMat[0][0]);
		glProgramUniform2fv(animQuadShader, animQuadUVset, 1, (GLfloat*)&objects[n].offsets[0]);

		glDrawArrays(GL_POINTS, 0, 1);
	}
}