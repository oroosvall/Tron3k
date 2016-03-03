#include "AnimatedMesh_v2.h"
#include <fstream>

#include "../Texture.h"

#include "../Streaming/TextureStreamer.h"

#include <Windows.h>

using std::ios;


void PlayerMesh::load(string fileName, std::string _character)
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
				tex[i].textureName = std::string("GameFiles/Textures/" + _character + "/" + std::string(texName, textHeader[i].textureSize));
				delete[] texName;
			}
			delete[] textHeader;

			for (int i = 0; i < textureCount; i++)
			{
				//tex[i].textureID = loadTexture(tex[i].textureName);
				//addToStreamQueue(&tex[i].textureID, std::string(tex[i].textureName));
				tex[i].textureID = TextureManager::gTm->createTexture(tex[i].textureName);
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

	delete[] matOffsets;
	delete[] indices;
	delete[] verts;

}

void PlayerMesh::release()
{
	
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

void PlayerMesh::render(GLuint shader, GLuint textureLocation, GLuint normalLocation, GLuint glowSpecLocation)
{
	//if (GetAsyncKeyState(VK_F8))
	//{
	//	if (materials[0].textureMapIndex != -1)
	//	{
	//		glDeleteTextures(1, &tex[materials[0].textureMapIndex].textureID);
	//		tex[materials[0].textureMapIndex].textureID = loadTexture(tex[materials[0].textureMapIndex].textureName);
	//	}
	//
	//	if (materials[0].normalMapIndex != -1)
	//	{
	//		glDeleteTextures(1, &tex[materials[0].normalMapIndex].textureID);
	//		tex[materials[0].normalMapIndex].textureID = loadTexture(tex[materials[0].normalMapIndex].textureName);
	//	}
	//	if (materials[0].specularMapIndex != -1)
	//	{
	//		glDeleteTextures(1, &tex[materials[0].specularMapIndex].textureID);
	//		tex[materials[0].specularMapIndex].textureID = loadTexture(tex[materials[0].specularMapIndex].textureName);
	//	}
	//}
	//
	//if (tex)
	//{
	//	glActiveTexture(GL_TEXTURE0);
	//	if (materials[0].textureMapIndex != -1)
	//	{
	//		glBindTexture(GL_TEXTURE_2D, tex[materials[0].textureMapIndex].textureID);
	//	}
	//	else
	//	{
	//		glBindTexture(GL_TEXTURE_2D, blank_normal);
	//	}
	//	glActiveTexture(GL_TEXTURE0 + 1);
	//	if (materials[0].normalMapIndex != -1)
	//	{
	//		glBindTexture(GL_TEXTURE_2D, tex[materials[0].normalMapIndex].textureID);
	//	}
	//	else
	//	{
	//		glBindTexture(GL_TEXTURE_2D, blank_normal);
	//	}
	//	glActiveTexture(GL_TEXTURE0 + 2);
	//	if (materials[0].specularMapIndex != -1)
	//	{
	//		glBindTexture(GL_TEXTURE_2D, tex[materials[0].specularMapIndex].textureID);
	//	}
	//	else
	//	{
	//		glBindTexture(GL_TEXTURE_2D, blank_glow);
	//	}
	//}

	glActiveTexture(GL_TEXTURE0);
	glProgramUniform1i(shader, textureLocation, 0);

	if (materials[0].textureMapIndex != -1)
		TextureManager::gTm->bindTextureOnly(tex[materials[0].textureMapIndex].textureID, DIFFUSE_FB);
	else
		TextureManager::gTm->bindDefaultOnly(DIFFUSE_FB);
	
	glActiveTexture(GL_TEXTURE1);
	glProgramUniform1i(shader, normalLocation, 1);
	if (materials[0].normalMapIndex != -1)
		TextureManager::gTm->bindTextureOnly(tex[materials[0].normalMapIndex].textureID, NORMAL_FB);
	else
		TextureManager::gTm->bindDefaultOnly(NORMAL_FB);

	glActiveTexture(GL_TEXTURE2);
	glProgramUniform1i(shader, glowSpecLocation, 2);
	if (materials[0].specularMapIndex != -1)
		TextureManager::gTm->bindTextureOnly(tex[materials[0].specularMapIndex].textureID, GLOW_FB);
	else
		TextureManager::gTm->bindDefaultOnly(GLOW_FB);

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
	if (hasSecondary)
		secondary.release();

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

	third.load(file + "_third.bin", character);
	first.load(file + "_first.bin", character);
	if (character == "brute")
	{
		secondary.load(file + "_secondary.bin", character);
		hasSecondary = true;
	}

	glGenBuffers(1, &matricesBuffer);
	glBindBuffer(GL_UNIFORM_BUFFER, matricesBuffer);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) * animations[0].header.jointCount, (void*)1, GL_STATIC_DRAW);
}

int* AnimatedMeshV2::loadAnimations(std::string character)
{
	std::string file = "GameFiles/CharacterFiles/" + character + "/anim_" + character;

	animations[AnimationState::first_primary_idle			].load(file + "_first_primary_idle.bin");
	animations[AnimationState::first_primary_run			].load(file + "_first_primary_run.bin");
	animations[AnimationState::first_primary_fire			].load(file + "_first_primary_fire.bin");
	animations[AnimationState::first_primary_reload			].load(file + "_first_primary_reload.bin");
	animations[AnimationState::first_primary_throw			].load(file + "_first_primary_throw.bin");
	animations[AnimationState::first_primary_switch			].load(file + "_first_primary_switch.bin");
	animations[AnimationState::first_primary_switch_IN		].load(file + "_first_primary_switch_IN.bin");
																						
	animations[AnimationState::first_secondary_idle			].load(file + "_first_secondary_idle.bin");
	animations[AnimationState::first_secondary_run			].load(file + "_first_secondary_run.bin");
	animations[AnimationState::first_secondary_fire			].load(file + "_first_secondary_fire.bin");
	animations[AnimationState::first_secondary_reload		].load(file + "_first_secondary_reload.bin");
	animations[AnimationState::first_secondary_throw		].load(file + "_first_secondary_throw.bin");
	animations[AnimationState::first_secondary_switch		].load(file + "_first_secondary_switch.bin");
	animations[AnimationState::first_secondary_switch_IN	].load(file + "_first_secondary_switch_IN.bin");
	
	animations[AnimationState::first_secondary_idle_left	].load(file + "_first_secondary_idle_left.bin");
	animations[AnimationState::first_secondary_run_left		].load(file + "_first_secondary_run_left.bin");
	animations[AnimationState::first_secondary_fire_left	].load(file + "_first_secondary_fire_left.bin");
	animations[AnimationState::first_secondary_death_left	].load(file + "_first_secondary_death_left.bin");

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
																					
	animations[AnimationState::third_shankbot_melee_standing].load(file + "_Third_Secondary_fire.bin");
	animations[AnimationState::third_shankbot_melee_run		].load(file + "_Third_Secondary_fire.bin");
	animations[AnimationState::third_shankbot_charge		].load(file + "_third_primary_charge.bin");
	animations[AnimationState::third_shankbot_walljump_right].load(file + "_third_primary_walljump_left.bin");
	animations[AnimationState::third_shankbot_walljump_left	].load(file + "_third_primary_walljump_right.bin");
																				
	animations[AnimationState::third_primary_death			].load(file + "_third_primary_death.bin");
	animations[AnimationState::third_secondary_death		].load(file + "_third_secondary_death.bin");

	animations[AnimationState::first_primary_death			].load(file + "_first_primary_death.bin");
	animations[AnimationState::first_secondary_death		].load(file + "_first_secondary_death.bin");

	animations[AnimationState::dance						].load(file + "_third_dance.bin");

	for (int i = 0; i < AnimationState::none; i++)
	{
		frames[i] = animations[i].header.keyCount;
	}
	return frames;
}

void AnimatedMeshV2::draw(GLuint uniformKeyMatrixLocation, int animationID, int keyFrame, bool _first, bool _primary, GLuint shader, GLuint textureLocation, GLuint normalLocation, GLuint glowSpecLocation)
{
	

	if (animations[animationID].header.keyCount > 0 && animations[animationID].header.jointCount > 0)
	{
		glBindBuffer(GL_UNIFORM_BUFFER, matricesBuffer);
		glBindBufferBase(GL_UNIFORM_BUFFER, uniformKeyMatrixLocation, matricesBuffer);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4)* animations[animationID].header.jointCount, animations[animationID].keyFrames[keyFrame].jointTransform, GL_STATIC_DRAW);

		if (_first)
		{
			if(_primary)
				first.render(shader, textureLocation, normalLocation, glowSpecLocation);
			else
				secondary.render(shader, textureLocation, normalLocation, glowSpecLocation);
		}
		else
			third.render(shader, textureLocation, normalLocation, glowSpecLocation);
	}
}
