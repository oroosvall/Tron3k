#include "Map.h"
#include <fstream>
#include <iostream>

void Map::init()
{
	currentChunk = -1;

	loadMap("GameFiles/TestFiles/Tron3k_map_3_allStatics.bin");

	for (int i = 0; i < meshCount; i++)
	{
		meshes[i].stream();
	}
	for (int i = 0; i < textureCount; i++)
	{
		tex[i].textureID = loadTexture("GameFiles/testfiles/" + std::string(tex[i].textureName));
	}

	portals.push_back(PortalData());
	portals[0].init(0, 0, 1, vec3(5, -10, 10), vec3(10, -10, 10), vec3(10, -15, 10), vec3(5, -15, 10));

	portals.push_back(PortalData());
	portals[1].init(0, 0, 1, vec3(5, 15.1, 15), vec3(10, 15.1, 15), vec3(10, 15.2, 10), vec3(5, 15.2, 10));
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

	if (materials)
	{
		delete[] materials;
		materials = nullptr;
	}

	if (tex)
	{
		delete[] tex;
		tex = nullptr;
	}

	//if (bbPoints)
	//{
	//	delete[] bbPoints;
	//	bbPoints = nullptr;
	//}

}

void Map::render(GLuint shader, GLuint shaderLocation, GLuint texture, GLuint normal, GLuint spec)
{
	for (int i = 0; i < meshCount; i++)
	{
		for (int ins = 0; ins < meshes[i].instanceCount; ins++)
		{

			glProgramUniformMatrix4fv(shader, shaderLocation, 1, GL_TRUE, (GLfloat*)&meshes[i].worldMatrices[ins][0][0]);

			glProgramUniform1i(shader, texture, 0);
			glProgramUniform1i(shader, normal, 1);
			glProgramUniform1i(shader, spec, 2);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, tex[materials[meshes[i].material].textureMapIndex].textureID);
			glActiveTexture(GL_TEXTURE0 + 1);
			glBindTexture(GL_TEXTURE_2D, tex[materials[meshes[i].material].normalMapIndex].textureID);
			glActiveTexture(GL_TEXTURE0 + 2);
			glBindTexture(GL_TEXTURE_2D, tex[materials[meshes[i].material].specularMapIndex].textureID);

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
	int materialCount = (int)fileHeader.materialCount;
	textureCount = (int)fileHeader.textureCount;
	int portalCount = (int)fileHeader.portalCount;
	int capCount = (int)fileHeader.capturePointCount;
	int spTACount = (int)fileHeader.SPCountTeamA;
	int spTBCount = (int)fileHeader.SPCountTeamB;
	int spFFACount = (int)fileHeader.SPCountTeamFFA;
	
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
		meshes[i].material = materialIndices[0];
		delete[]materialIndices;
	
		int* materialOffsets = new int[materialCount];
		inFile.read((char*)materialOffsets, sizeof(int)*materialCount);
		delete[]materialOffsets;
	
		int* indices = new int[indicesCount];
		inFile.read((char*)indices, sizeof(int)*indicesCount);
		meshes[i].setIndices(indices, indicesCount);
	
		Vertex11* verts = new Vertex11[vertexCount];
		inFile.read((char*)verts, sizeof(Vertex11)*vertexCount);
		meshes[i].setVertices((float*&)verts, vertexCount);
	
		AAB* bounds = new AAB[instanceCount];
		inFile.read((char*)bounds, sizeof(AAB)*instanceCount);
		delete [] bounds;
	
		bbPoints = new BBPoint[bbCount];
		inFile.read((char*)bbPoints, sizeof(BBPoint)*bbCount);
	
		// temp fix for leaks
		delete[] bbPoints;

		for (int i = 0; i < instanceCount; i++)
		{
			glm::mat4* bbMats = new glm::mat4[bbCount];
			inFile.read((char*)bbMats, sizeof(glm::mat4)*bbCount);
			std::cout << std::endl;
			delete[] bbMats;
		}
	}
	
	materials= new Material[materialCount];
	inFile.read((char*)materials, sizeof(Material) * materialCount);
	
	tex = new TextureStruct[textureCount];
	TextureHDR* textHeader = new TextureHDR[textureCount];
	inFile.read((char*)textHeader, sizeof(TextureHDR) * textureCount);
	
	for (int i = 0; i < textureCount; i++)
	{
		char* texName = new char[(unsigned int)textHeader[i].textureSize];
		inFile.read(texName, sizeof(char) * (unsigned int)textHeader[i].textureSize);
		tex[i].textureName = std::string(texName, textHeader[i].textureSize);
		delete[] texName;
	}
	delete[] textHeader;
	
	PointLight* pl = new PointLight[pointLightCount];
	inFile.read((char*)pl, sizeof(PointLight) * pointLightCount);
	delete[] pl;

	SpotLightH* sl = new SpotLightH[spotLightCount];
	inFile.read((char*)sl, sizeof(SpotLightH) * spotLightCount);
	delete[] sl;

	PortalDataRead* portalData = new PortalDataRead[portalCount];
	inFile.read((char*)portalData, sizeof(PortalDataRead) * portalCount);
	delete[] portalData;
	
	inFile.close();

}

int Map::getChunkID(glm::vec3 oldPos, glm::vec3 newPos)
{
	vec3 dir = newPos - oldPos;
	float len = length(dir);
	dir = normalize(dir);

	for (int n = 0; n < portals.size(); n++)
		if (portals[n].intersects(oldPos, dir, len))
		{
			if (portals[n].bridgedRooms[0] == currentChunk)
				currentChunk = portals[n].bridgedRooms[1];
			else
				currentChunk = portals[n].bridgedRooms[0];

			printf("Now in room %d \n", currentChunk);

			return currentChunk;
		}

	return currentChunk;
}

void Map::renderPortals()
{
	for (int n = 0; n < portals.size(); n++)
		portals[n].render();
}