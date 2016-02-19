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

/*	AnimatedObject sign;
	sign.type = 0;
	sign.maxtimer = 1.0f;
	sign.timer = sign.maxtimer;
	sign.worldMat = mat4(1.0f);
	objects.push_back(sign);
	*/

	AnimatedObject water;
	water.type = 1;
	water.worldMat = mat4(1.0f);
	objects.push_back(water);

	//youmuanim
	test.worldMat[0] = glm::vec4(0,5,0,1);
	test.worldMat[1] = glm::vec4(0,0,0,1);
	test.worldMat[2] = glm::vec4(5,5,0,1);
	test.worldMat[3] = glm::vec4(5,0,0,1);
	test.segments = 5.0f;
	test.offsets.x = 0;
	test.offsets.y = 1.0f / test.segments;
	test.freezeTimer = 0.2f;
	test.swapTimer = 0.0f;
	//TextureManager::
	test.timer = test.freezeTimer;
}

void AnimatedTexture::Update(float dT)
{
	for (size_t i = 0; i < objects.size(); i++)
	{
		AnimatedObject* o = &objects[i];

		//Timer calculations
		o->timer -= dT;
		o->update(dT);
	}

}

void AnimatedTexture::render()
{
	for (size_t i = 0; i < objects.size(); i++)
	{
		AnimatedObject* o = &objects[i];
		glUseProgram(animQuadShader);
		glDisable(GL_CULL_FACE);

		glProgramUniformMatrix4fv(animQuadShader, animQuadWorld, 1, GL_FALSE, (GLfloat*)&o->worldMat[0][0]);
		glProgramUniform2fv(animQuadShader, animQuadUVset, 1, (GLfloat*)&o->offsets[0]);


		glDrawArrays(GL_POINTS, 0, 1);
	}

}