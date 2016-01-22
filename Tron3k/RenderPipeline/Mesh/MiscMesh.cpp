#include "MiscMesh.h"
#include <fstream>

#include "../Texture.h"

using std::ios;

void MiscMesh::init()
{

}

void MiscMesh::release()
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

void MiscMesh::load(std::string fileName)
{

	std::ifstream file;
	file.open(fileName, ios::in | ios::binary);

	if (file.is_open())
	{

		StaticMeshHeader hdr;
		file.read((char*)&hdr, sizeof(StaticMeshHeader));

		//glm::mat4 transform;
		//file.read((char*)&transform, sizeof(glm::mat4));

		matOffsets = new int[hdr.materialCount];
		file.read((char*)matOffsets, sizeof(int)*hdr.materialCount);

		indices = new int[hdr.indexCount];
		file.read((char*)indices, sizeof(int)*hdr.indexCount);

		verts = new Vertex11[hdr.vertexCount];
		file.read((char*)verts, sizeof(Vertex11)*hdr.vertexCount);

		materials = new Material[hdr.materialCount];
		file.read((char*)materials, sizeof(Material) * hdr.materialCount);

		textureCount = hdr.textureCount;

		if (textureCount > 0)
		{
			tex = new TextureStruct[hdr.textureCount];
			TextureHDR* textHeader = new TextureHDR[hdr.textureCount];
			file.read((char*)textHeader, sizeof(TextureHDR) * hdr.textureCount);

			for (uint32_t i = 0; i < hdr.textureCount; i++)
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
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex11)* hdr.vertexCount, verts, GL_STATIC_DRAW);

		glGenBuffers(1, &ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * hdr.indexCount, indices, GL_STATIC_DRAW);
		//glEnableVertexAttribArray(index);

		indexCount = hdr.indexCount;

		//define vertex data layout
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);

#define BUFFER_OFFSET(i) ((char *)nullptr + (i))

		//pos
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex11), BUFFER_OFFSET(0));
		//uv
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex11), BUFFER_OFFSET(sizeof(float) * 3));
		// normal
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex11), BUFFER_OFFSET(sizeof(float) * 5));
		// tangent
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex11), BUFFER_OFFSET(sizeof(float) * 8));

		file.close();
	}

}

void MiscMesh::draw()
{
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
