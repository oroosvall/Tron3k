#include "AnimatedMesh.h"
#include <fstream>
#include <iostream>

using std::ios;

void AnimatedMesh::init()
{
	activeAnimation = 0;
	currentKeyFrame = 0;
}

void AnimatedMesh::release()
{
	delete[] matOffsets;
	delete[] indices;
	delete[] verts;

	for (int32_t i = 0; i < animationCount; i++)
	{
		for (int32_t j = 0; j < animationKeyCounts[i]; j++)
		{
			delete[] animations[i].keyFrames[j].jointTransform;
		}
		delete[] animations[i].keyFrames;
	}
	delete[] animations;

	delete[] animationKeyCounts;
	delete[] animationType;
	
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ibo);
}

AnimatedMesh::~AnimatedMesh()
{

}

void AnimatedMesh::update(float deltaTime)
{
	activeAnimation;

	currentKeyFrame++;

	if (currentKeyFrame >= animationKeyCounts[activeAnimation])
	{
		currentKeyFrame = 0;
	}

	memcpy(matrixBufferDataOneKey, animations[0].keyFrames[currentKeyFrame].jointTransform, sizeof(glm::mat4)*jointCount);
	glBindBuffer(GL_UNIFORM_BUFFER, matricesBuffer);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(animationMatrices)* jointCount, matrixBufferDataOneKey, GL_STATIC_DRAW);

	using namespace std;

	if (false)
	{
		for (size_t k = 0; k < jointCount; k++)
		{
			for (size_t y = 0; y < 4; y++)
			{
				for (size_t x = 0; x < 4; x++)
				{
					cout << animations[0].keyFrames[currentKeyFrame].jointTransform[k][y][x] << " ";
				}
				cout << endl;
			}
			cout << endl;
		}
	}
	
}

void AnimatedMesh::load(std::string fileName)
{

	std::ifstream file;
	file.open(fileName, ios::in | ios::binary);

	if (file.is_open())
	{

		CharacterHeader charHdr;
		file.read((char*)&charHdr, sizeof(CharacterHeader));

		glm::mat4 transform;
		file.read((char*)&transform, sizeof(glm::mat4));

		matOffsets = new int[charHdr.materialCount];
		file.read((char*)matOffsets, sizeof(int)*charHdr.materialCount);
		int a = 0;

		indices = new int[charHdr.indicesCount];
		file.read((char*)indices, sizeof(int)*charHdr.indicesCount);
		int b = 0;

		verts = new AnimVertex[charHdr.vertexCount];
		file.read((char*)verts, sizeof(AnimVertex)*charHdr.vertexCount);
		int c = 0;

		animationKeyCounts = new int[charHdr.animationCount];
		file.read((char*)animationKeyCounts, sizeof(int)*charHdr.animationCount);
		animationCount = charHdr.animationCount;
		jointCount = charHdr.jointCount;

		animationType = new int[charHdr.animationCount];
		file.read((char*)animationType, sizeof(int)*charHdr.animationCount);

		animations = new Animation[charHdr.animationCount];

		for (uint32_t i = 0; i < charHdr.animationCount; i++)
		{
			animations[i].keyFrames = new Keyframe[animationKeyCounts[i]];
			for (int32_t j = 0; j < animationKeyCounts[i]; j++)
			{
				animations[i].keyFrames[j].jointTransform = new glm::mat4[charHdr.jointCount];
				file.read((char*)animations[i].keyFrames[j].jointTransform, sizeof(glm::mat4) * charHdr.jointCount);
			}
		}

		matrixBufferDataOneKey = new animationMatrices[charHdr.jointCount];

		memcpy(matrixBufferDataOneKey, animations[0].keyFrames[0].jointTransform, sizeof(glm::mat4)*charHdr.jointCount);


		Material* materials = new Material[charHdr.materialCount];
		file.read((char*)materials, sizeof(Material) * charHdr.materialCount);
		delete[] materials;

		TextureStruct* tex = new TextureStruct[charHdr.textureCount];
		TextureHDR* textHeader = new TextureHDR[charHdr.textureCount];
		file.read((char*)textHeader, sizeof(TextureHDR) * charHdr.textureCount);

		for (uint32_t i = 0; i < charHdr.textureCount; i++)
		{
			char* texName = new char[(unsigned int)textHeader[i].textureSize];
			file.read(texName, sizeof(char) * (unsigned int)textHeader[i].textureSize);
			tex[i].textureName = std::string(texName, textHeader[i].textureSize);
			delete[] texName;
		}
		delete[] textHeader;
		delete[] tex;

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

		glGenBuffers(1, &matricesBuffer);
		glBindBuffer(GL_UNIFORM_BUFFER, matricesBuffer);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(animationMatrices)* jointCount, matrixBufferDataOneKey, GL_STATIC_DRAW);

	}
}

void AnimatedMesh::draw(GLuint uniformKeyMatrixLocation)
{
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

	glBindBuffer(GL_UNIFORM_BUFFER, matricesBuffer);
	glBindBufferBase(GL_UNIFORM_BUFFER, uniformKeyMatrixLocation, matricesBuffer);

	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

}