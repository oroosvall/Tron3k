#include "Map.h"
#include <fstream>
#include <iostream>

void Map::init()
{
	loadMap("GameFiles/TestFiles/Tron3k_map_0.bin");

	for (int i = 0; i < meshCount; i++)
	{
		meshes[i].stream();
	}

}

void Map::release()
{
	if (meshes)
	{
		for (int i = 0; i < meshCount; i++)
		{
			meshes[i].release();
		}

		delete[] meshes;
		meshes = nullptr;
	}

	if (bbPoints)
	{
		delete[] bbPoints;
		bbPoints = nullptr;
	}

}

void Map::render(GLuint shader, GLuint shaderLocation)
{
	for (int i = 0; i < meshCount; i++)
	{
		for (int ins = 0; ins < meshes[i].instanceCount; ins++)
		{

			glProgramUniformMatrix4fv(shader, shaderLocation, 1, GL_TRUE, (GLfloat*)&meshes[i].worldMatrices[ins][0][0]);

			//glProgramUniform1i(shader, textureLocation, 0);
			//glProgramUniform1i(shader, normalLocation, 1);
			//glActiveTexture(GL_TEXTURE0);
			//glBindTexture(GL_TEXTURE_2D, meshes[i].textureID);
			//
			//glActiveTexture(GL_TEXTURE0 + 1);
			//glBindTexture(GL_TEXTURE_2D, textures[2].textureID);

			glBindVertexArray(meshes[i].vertexArray);
			glBindBuffer(GL_ARRAY_BUFFER, meshes[i].vertexBuffer);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshes[i].indexBuffer);

			glDrawElements(GL_TRIANGLES, meshes[i].indexCount, GL_UNSIGNED_INT, 0);
		}
		
	}
}

void Map::loadMap(std::string mapName)
{
	using namespace std;

	ifstream inFile;
	inFile.open(mapName, ios::in | ios::binary);

	SharedFileHDR fileHeader;

	inFile.read((char*)&fileHeader, sizeof(fileHeader));

	cout << sizeof(fileHeader) << endl;

	meshCount = (int)fileHeader.meshCount;
	int pointLightCount = (int)fileHeader.pointLightCount;
	int spotLightCount = (int)fileHeader.spotLightCount;
	int dirLightCount = (int)fileHeader.dirLightCount;
	int materialCount = (int)fileHeader.materialCount;
	int textureCount = (int)fileHeader.textureCount;
	int portalCount = (int)fileHeader.portalCount;

	meshes = new StaticMesh[meshCount];

	for (int i = 0; i < meshCount; i++)
	{
		MeshDataHDR meshHeader;
		inFile.read((char*)&meshHeader, sizeof(meshHeader));

		int objType = meshHeader.objectType;
		int instanceCount = meshHeader.instanceCount;
		int materialCount = meshHeader.materialCount;
		int indicesCount = meshHeader.indicesCount;
		int vertexCount = meshHeader.vertexCount;
		int bbCount = meshHeader.bbCount;

		int* roomIDs = new int[instanceCount];
		meshes[i].init(instanceCount);
		inFile.read((char*)roomIDs, sizeof(int)*instanceCount);
		meshes[i].setRoomIDs(roomIDs);

		glm::mat4* matrices = new glm::mat4[instanceCount];

		inFile.read((char*)matrices, sizeof(glm::mat4)*instanceCount);
		meshes[i].setMatrices(matrices);

		int* materialIndices = new int[materialCount];
		inFile.read((char*)materialIndices, sizeof(int)*materialCount);
		delete[]materialIndices;

		int* materialOffsets = new int[materialCount];
		inFile.read((char*)materialOffsets, sizeof(int)*materialCount);
		delete[]materialOffsets;

		int* indices = new int[indicesCount];
		inFile.read((char*)indices, sizeof(int)*indicesCount);
		meshes[i].setIndices(indices, indicesCount);

		Vertex11* verts = new Vertex11[vertexCount];
		inFile.read((char*)verts, sizeof(Vertex11)*vertexCount);
		meshes[i].setVertices((float*)verts, vertexCount);

		bbPoints = new BBPoint[bbCount];
		inFile.read((char*)bbPoints, sizeof(BBPoint)*bbCount);

		for (int i = 0; i < instanceCount; i++)
		{
			glm::mat4* bbMats = new glm::mat4[bbCount];
			inFile.read((char*)bbMats, sizeof(glm::mat4)*bbCount);
			std::cout << std::endl;
			delete[] bbMats;
		}
	}

	PointLight* pl = new PointLight[pointLightCount];
	inFile.read((char*)pl, sizeof(PointLight) * pointLightCount);
	delete[] pl;

	SpotLightH* sl = new SpotLightH[spotLightCount];
	inFile.read((char*)sl, sizeof(SpotLightH) * spotLightCount);
	delete[] sl;

	DirectionalLight* dl = new DirectionalLight[dirLightCount];
	inFile.read((char*)dl, sizeof(DirectionalLight) * dirLightCount);
	delete[] dl;

	Material* mats = new Material[materialCount];
	inFile.read((char*)mats, sizeof(Material) * materialCount);
	delete[]mats;

	TextureHDR* textHeader = new TextureHDR[textureCount];
	TextureData* texData = new TextureData[textureCount];
	inFile.read((char*)textHeader, sizeof(TextureHDR) * textureCount);
	
	for (int i = 0; i < textureCount; i++)
	{
		texData[i].texturePath = new char[(unsigned int)textHeader[i].textureSize];
		inFile.read((char*)texData[i].texturePath, sizeof(char) * (unsigned int)textHeader[i].textureSize);
		delete[] texData[i].texturePath;
	}
	delete[] texData;
	delete[] textHeader;

	PortalData* portalData = new PortalData[portalCount];
	inFile.read((char*)portalData, sizeof(PortalData) * portalCount);
	delete[] portalData;

	inFile.close();

}
