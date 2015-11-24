#include "Map.h"
#include <fstream>
#include <iostream>

void Map::init()
{
	loadMap("GameFiles/TestFiles/Tron3k_map_0.bin");
}

void Map::release()
{
	if (meshes)
	{
		delete[] meshes;
		meshes = nullptr;
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

	int meshCount = (int)fileHeader.meshCount;
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

		cout << sizeof(meshHeader) << endl;

		int objType = meshHeader.objectType;
		int instanceCount = meshHeader.instanceCount;
		int materialCount = meshHeader.materialCount;
		int indicesCount = meshHeader.indicesCount;
		int vertexCount = meshHeader.vertexCount;
		int bbCount = meshHeader.bbCount;

		meshes[i].init(instanceCount);

		int* roomIDs = new int[instanceCount];
		inFile.read((char*)roomIDs, sizeof(int)*instanceCount);
		meshes[i].setRoomIDs(roomIDs);

		glm::mat4* matrices = new glm::mat4[instanceCount];

		inFile.read((char*)matrices, sizeof(glm::mat4)*instanceCount);
		meshes[i].setMatrices(matrices);

		int* materialIndices = new int[instanceCount];
		inFile.read((char*)materialIndices, sizeof(int)*instanceCount);
		delete[]materialIndices;

		int* materialOffsets = new int[instanceCount];
		inFile.read((char*)materialOffsets, sizeof(int)*instanceCount);
		delete[]materialOffsets;

		int* indices = new int[indicesCount];
		inFile.read((char*)indices, sizeof(int)*indicesCount);
		meshes[i].setIndices(indices, indicesCount);

		Vertex11* verts = new Vertex11[vertexCount];
		inFile.read((char*)verts, sizeof(Vertex11)*vertexCount);
		meshes[i].setVertices((float*)verts, vertexCount);

		struct BBPoint
		{
			Vertex BBPos[8];
		};

		BBPoint* bbPoints = new BBPoint[bbCount];
		inFile.read((char*)bbPoints, sizeof(Vertex11)*bbCount);
		delete[] bbPoints;

	}

	
	


}
