#include "AnimatedTexture.h"

AnimatedTexture::AnimatedTexture()
{
}

AnimatedTexture::~AnimatedTexture()
{
}

void AnimatedTexture::Initialize()
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
	water.maxtimer = 1.0f;
	water.worldMat = mat4(1.0f);
	objects.push_back(water);

	test.worldMat[0] = glm::vec4(0,5,0,1);
	test.worldMat[1] = glm::vec4(0,0,0,1);
	test.worldMat[2] = glm::vec4(5,5,0,1);
	test.worldMat[3] = glm::vec4(5,0,0,1);
	test.offsets.x = 0;
	test.offsets.y = 1;
}

void AnimatedTexture::Update(float dT)
{
	//for (int i = 0; i < objects.size(); i++)
	//{
	//	AnimatedObject* o = &objects[i];
	//
	//	//Timer calculations
	//	o->timer -= dT;
	//
	//	//Sign
	//	if (o->type == 0)
	//	{
	//		if (o->timer < 0)
	//		{
	//			//swap
	//			o->timer = o->maxtimer;
	//
	//			//Set offset
	//			float movement = 1.0f / o->segments;
	//			o->offset += movement;
	//		}
	//	}
	//
	//	//Water
	//	else
	//	{
	//		o->offset += 0.001f * dT;
	//	}
	//}
}

void AnimatedTexture::render()
{
	glUseProgram(animQuadShader);
	glDisable(GL_CULL_FACE);

	glProgramUniformMatrix4fv(animQuadShader, animQuadWorld, 1, GL_FALSE, (GLfloat*)&test.worldMat[0][0]);
	glProgramUniform2fv(animQuadShader, animQuadUVset, 1, (GLfloat*)&test.offsets[0]);

	glDrawArrays(GL_POINTS, 0, 1);

}