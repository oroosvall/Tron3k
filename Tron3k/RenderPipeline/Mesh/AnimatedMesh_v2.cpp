#include "AnimatedMesh_v2.h"
#include <fstream>

#include "../Texture.h"

#include <Windows.h>

using std::ios;

void AnimatedMeshV2::init()
{
}

void AnimatedMeshV2::release()
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
	
	if(materials)
		delete[] materials;

}

AnimatedMeshV2::~AnimatedMeshV2()
{
}

void AnimatedMeshV2::load(std::string fileName)
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

void AnimatedMeshV2::draw(GLuint uniformKeyMatrixLocation)
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