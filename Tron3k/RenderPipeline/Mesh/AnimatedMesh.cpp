#include "AnimatedMesh.h"
#include <fstream>

#include "../Texture.h"

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
	
	delete[] matrixBufferDataOneKey;

	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ibo);
	glDeleteBuffers(1, &matricesBuffer);

	for (int i = 0; i < textureCount; i++)
	{
		glDeleteTextures(1, &tex[i].textureID);
	}
	if(tex)
		delete[] tex;
}

AnimatedMesh::~AnimatedMesh()
{

}

void AnimatedMesh::update(int setFrame)
{
	currentKeyFrame = setFrame;

	if (currentKeyFrame >= animationKeyCounts[activeAnimation])
	{
		currentKeyFrame = 0;
	}

	memcpy(matrixBufferDataOneKey, animations[0].keyFrames[currentKeyFrame].jointTransform, sizeof(glm::mat4)*jointCount);
	glBindBuffer(GL_UNIFORM_BUFFER, matricesBuffer);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(animationMatrices)* jointCount, matrixBufferDataOneKey, GL_STATIC_DRAW);
	
}

void AnimatedMesh::setAnimation(int animID)
{
	if (animID < animationCount)
	{
		activeAnimation = animID;
		currentKeyFrame = animationKeyCounts[activeAnimation];
		update(0);
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
				tex[i].textureName = "GameFiles/TestFiles/" + std::string(texName, textHeader[i].textureSize);
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

		glGenBuffers(1, &matricesBuffer);
		glBindBuffer(GL_UNIFORM_BUFFER, matricesBuffer);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(animationMatrices)* jointCount, matrixBufferDataOneKey, GL_STATIC_DRAW);

	}
}

void AnimatedMesh::draw(GLuint uniformKeyMatrixLocation)
{
	glActiveTexture(GL_TEXTURE0);
	if(tex)
		glBindTexture(GL_TEXTURE_2D, tex[0].textureID);
	else
		glBindTexture(GL_TEXTURE_2D, blank_diffuse);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

	glBindBuffer(GL_UNIFORM_BUFFER, matricesBuffer);
	glBindBufferBase(GL_UNIFORM_BUFFER, uniformKeyMatrixLocation, matricesBuffer);

	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
	
}