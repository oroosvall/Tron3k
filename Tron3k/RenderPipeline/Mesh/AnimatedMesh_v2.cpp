#include "AnimatedMesh_v2.h"
#include <fstream>

#include "../Texture.h"

#include <Windows.h>

using std::ios;


void PlayerMesh::load(string fileName)
{
	std::ifstream file;
	file.open(fileName, ios::in | ios::binary);

	if (file.is_open())
	{

		CharacterHeaderV2 charHdr;
		file.read((char*)&charHdr, sizeof(CharacterHeaderV2));

		//glm::mat4 transform;
		//file.read((char*)&transform, sizeof(glm::mat4));

		matOffsets = new int[charHdr.materialCount];
		file.read((char*)matOffsets, sizeof(int)*charHdr.materialCount);

		indices = new int[charHdr.indicesCount];
		file.read((char*)indices, sizeof(int)*charHdr.indicesCount);

		verts = new AnimVertex[charHdr.vertexCount];
		file.read((char*)verts, sizeof(AnimVertex)*charHdr.vertexCount);

		materials = new Material[charHdr.materialCount];
		file.read((char*)materials, sizeof(Material) * charHdr.materialCount);

		textureCount = charHdr.textureCount;

		if (textureCount > 0)
		{
			tex = new TextureStruct[charHdr.textureCount];
			TextureHDR* textHeader = new TextureHDR[charHdr.textureCount];
			file.read((char*)textHeader, sizeof(TextureHDR) * charHdr.textureCount);

			for (uint32_t i = 0; i < charHdr.textureCount; i++)
			{
				char* texName = new char[(unsigned int)textHeader[i].textureSize];
				file.read(texName, sizeof(char) * (unsigned int)textHeader[i].textureSize);
				tex[i].textureName = "GameFiles/Textures/" + std::string(texName, textHeader[i].textureSize);
				delete[] texName;
			}
			delete[] textHeader;

			for (int i = 0; i < textureCount; i++)
			{
				tex[i].textureID = loadTexture(tex[i].textureName);
			}
		}

		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(AnimVertex)* charHdr.vertexCount, verts, GL_STATIC_DRAW);

		glGenBuffers(1, &ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * charHdr.indicesCount, indices, GL_STATIC_DRAW);
		//glEnableVertexAttribArray(index);

		indexCount = charHdr.indicesCount;

		//define vertex data layout
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);
		glEnableVertexAttribArray(4);
		glEnableVertexAttribArray(5);

#define BUFFER_OFFSET(i) ((char *)nullptr + (i))

		//pos
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(AnimVertex), BUFFER_OFFSET(0));
		//uv
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(AnimVertex), BUFFER_OFFSET(sizeof(float) * 3));
		// normal
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(AnimVertex), BUFFER_OFFSET(sizeof(float) * 5));
		// tangent
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(AnimVertex), BUFFER_OFFSET(sizeof(float) * 8));
		// tangent
		glVertexAttribPointer(4, 4, GL_UNSIGNED_INT, GL_FALSE, sizeof(AnimVertex), BUFFER_OFFSET(sizeof(float) * 11));
		// tangent
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(AnimVertex), BUFFER_OFFSET(sizeof(float) * 15));

		file.close();
	}
}

void PlayerMesh::release()
{
	delete[] matOffsets;
	delete[] indices;
	delete[] verts;

	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ibo);

	for (int i = 0; i < textureCount; i++)
	{
		glDeleteTextures(1, &tex[i].textureID);
	}
	if (tex)
		delete[] tex;

	if (materials)
		delete[] materials;
}

void PlayerMesh::render()
{
	if (GetAsyncKeyState(VK_F8))
	{
		if (materials[0].textureMapIndex != -1)
		{
			glDeleteTextures(1, &tex[materials[0].textureMapIndex].textureID);
			tex[materials[0].textureMapIndex].textureID = loadTexture(tex[materials[0].textureMapIndex].textureName);
		}

		if (materials[0].normalMapIndex != -1)
		{
			glDeleteTextures(1, &tex[materials[0].normalMapIndex].textureID);
			tex[materials[0].normalMapIndex].textureID = loadTexture(tex[materials[0].normalMapIndex].textureName);
		}
		if (materials[0].specularMapIndex != -1)
		{
			glDeleteTextures(1, &tex[materials[0].specularMapIndex].textureID);
			tex[materials[0].specularMapIndex].textureID = loadTexture(tex[materials[0].specularMapIndex].textureName);
		}
	}

	if (tex)
	{
		glActiveTexture(GL_TEXTURE0);
		if (materials[0].textureMapIndex != -1)
		{
			glBindTexture(GL_TEXTURE_2D, tex[materials[0].textureMapIndex].textureID);
		}
		else
		{
			glBindTexture(GL_TEXTURE_2D, blank_normal);
		}
		glActiveTexture(GL_TEXTURE0 + 1);
		if (materials[0].normalMapIndex != -1)
		{
			glBindTexture(GL_TEXTURE_2D, tex[materials[0].normalMapIndex].textureID);
		}
		else
		{
			glBindTexture(GL_TEXTURE_2D, blank_normal);
		}
		glActiveTexture(GL_TEXTURE0 + 2);
		if (materials[0].specularMapIndex != -1)
		{
			glBindTexture(GL_TEXTURE_2D, tex[materials[0].specularMapIndex].textureID);
		}
		else
		{
			glBindTexture(GL_TEXTURE_2D, blank_glow);
		}
	}
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

}


void AnimatedMeshV2::init()
{
}

void AnimatedMeshV2::release()
{
	third.release();
	first.release();

	glDeleteBuffers(1, &matricesBuffer);

	for (int i = 0; i < AnimationState::none; i++)
	{
		animations[i].release();
	}	
}

AnimatedMeshV2::~AnimatedMeshV2()
{
}

void AnimatedMeshV2::update()
{

}

void AnimatedMeshV2::load(std::string character)
{
	std::string file = "GameFiles/CharacterFiles/" + character + "/mesh_" + character;

	third.load(file + "_third.bin");
	first.load(file + "_first.bin");
	glGenBuffers(1, &matricesBuffer);
	glBindBuffer(GL_UNIFORM_BUFFER, matricesBuffer);
	//glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4)* animIdle->header.jointCount, animIdle->keyFrames[0].jointTransform, GL_STATIC_DRAW);
}

int* AnimatedMeshV2::loadAnimations(std::string character)
{
	std::string file = "GameFiles/CharacterFiles/" + character + "/anim_" + character;

	animations[AnimationState::first_primary_idle			].load(file + "_first_primary_idle.bin");
	animations[AnimationState::first_primary_run			].load(file + "_first_primary_run.bin");
	animations[AnimationState::first_primary_fire			].load(file + "_first_primary_fire.bin");
	animations[AnimationState::first_primary_air			].load(file + "_first_primary_air.bin");
	animations[AnimationState::first_primary_reload			].load(file + "_first_primary_reload.bin");
	animations[AnimationState::first_primary_throw			].load(file + "_first_primary_throw.bin");
	animations[AnimationState::first_primary_switch			].load(file + "_first_primary_switch.bin");
																						
	animations[AnimationState::first_secondary_idle			].load(file + "_first_secondary_idle.bin");
	animations[AnimationState::first_secondary_run			].load(file + "_first_secondary_run.bin");
	animations[AnimationState::first_secondary_fire			].load(file + "_first_secondary_fire.bin");
	animations[AnimationState::first_secondary_air			].load(file + "_first_secondary_air.bin");
	animations[AnimationState::first_secondary_reload		].load(file + "_first_secondary_reload.bin");
	animations[AnimationState::first_secondary_throw		].load(file + "_first_secondary_throw.bin");
	animations[AnimationState::first_secondary_switch		].load(file + "_first_secondary_switch.bin");
																				
	animations[AnimationState::third_primary_idle			].load(file + "_third_primary_idle.bin");
	animations[AnimationState::third_primary_run			].load(file + "_third_primary_run.bin");
	animations[AnimationState::third_primary_run_rev		].load(file + "_third_primary_run_rev.bin");
	animations[AnimationState::third_primary_air			].load(file + "_third_primary_air.bin");
	animations[AnimationState::third_primary_strafe_left	].load(file + "_third_primary_strafe_left.bin");
	animations[AnimationState::third_primary_strafe_right	].load(file + "_third_primary_strafe_right.bin");
	animations[AnimationState::third_primary_jump_begin		].load(file + "_third_primary_jump_begin.bin");
	animations[AnimationState::third_primary_jump_end		].load(file + "_third_primary_jump_end.bin");
																						
	animations[AnimationState::third_secondary_idle			].load(file + "_third_secondary_idle.bin");
	animations[AnimationState::third_secondary_run			].load(file + "_third_secondary_run.bin");
	animations[AnimationState::third_secondary_run_rev		].load(file + "_third_secondary_run_rev.bin");
	animations[AnimationState::third_secondary_air			].load(file + "_third_secondary_air.bin");
	animations[AnimationState::third_secondary_strafe_left	].load(file + "_third_secondary_strafe_left.bin");
	animations[AnimationState::third_secondary_strafe_right	].load(file + "_third_secondary_strafe_right.bin");
	animations[AnimationState::third_secondary_jump_begin	].load(file + "_third_secondary_jump_begin.bin");
	animations[AnimationState::third_secondary_jump_end		].load(file + "_third_secondary_jump_end.bin");
																						
	animations[AnimationState::third_primary_switch			].load(file + "_third_primary_switch.bin");
	animations[AnimationState::third_secondary_switch		].load(file + "_third_secondary_switch.bin");
																				
	animations[AnimationState::third_brute_primary_dash		].load(file + "_Third_Primary_Dash.bin");
	animations[AnimationState::third_brute_secondary_dash	].load(file + "_Third_Secondary_Dash.bin");

	animations[AnimationState::third_turn_left				].load(file + "_third_turn_left.bin");
	animations[AnimationState::third_turn_right				].load(file + "_third_turn_right.bin");
																					
	animations[AnimationState::third_melee_standing			].load(file + "_third_melee_standing.bin");
	animations[AnimationState::third_melee_run				].load(file + "_third_melee_run.bin");
	animations[AnimationState::third_shankbot_charge		].load(file + "_third_shankbot_charge.bin");
																				
	animations[AnimationState::third_primary_death			].load(file + "_third_primary_death.bin");
	animations[AnimationState::third_secondary_death		].load(file + "_third_secondary_death.bin");


	int frames[AnimationState::none];

	for (int i = 0; i < AnimationState::none; i++)
	{
		frames[i] = animations[i].header.keyCount;
	}

	return frames;
}

void AnimatedMeshV2::draw(GLuint uniformKeyMatrixLocation,int animationID, int keyFrame, bool _first)
{

	glBindBuffer(GL_UNIFORM_BUFFER, matricesBuffer);
	glBindBufferBase(GL_UNIFORM_BUFFER, uniformKeyMatrixLocation, matricesBuffer);

	if (animations[animationID].header.keyCount > 0)
	{
		glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4)* animations[animationID].header.jointCount, animations[animationID].keyFrames[keyFrame].jointTransform, GL_STATIC_DRAW);
		if(_first)
			first.render();	
		else
			third.render();
	}


}
