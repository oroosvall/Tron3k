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
	portals[0].init(0, 0, 1, vec3(5, -10, 10), vec3(10, -15, 10), vec3(5, -20, 10), vec3(0, -15, 10));

	portals.push_back(PortalData());
	portals[1].init(0, 0, 1, vec3(5, 15., 15), vec3(10, 15., 15), vec3(10, 15, 10), vec3(5, 15, 10));
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

void Map::renderChunk(GLuint shader, GLuint shaderLocation, int chunkID)
{
	for (size_t i = 0; i < chunks[chunkID].props.size(); i++)
	{
		int meshID = chunks[chunkID].props[i].id;

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex[materials[meshes[meshID].material].textureMapIndex].textureID);
		glActiveTexture(GL_TEXTURE0 + 1);
		glBindTexture(GL_TEXTURE_2D, tex[materials[meshes[meshID].material].normalMapIndex].textureID);
		glActiveTexture(GL_TEXTURE0 + 2);
		glBindTexture(GL_TEXTURE_2D, tex[materials[meshes[meshID].material].specularMapIndex].textureID);
		
		glBindVertexArray(meshes[meshID].vertexArray);
		glBindBuffer(GL_ARRAY_BUFFER, meshes[meshID].vertexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshes[meshID].indexBuffer);

		for (int ins = 0; ins < chunks[chunkID].props[i].mats.size(); ins++)
		{
			glProgramUniformMatrix4fv(shader, shaderLocation, 1, GL_TRUE, (GLfloat*)&chunks[chunkID].props[i].mats[ins][0][0]);

			glDrawElements(GL_TRIANGLES, meshes[meshID].indexCount, GL_UNSIGNED_INT, 0);
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
	
	for (size_t i = 0; i < fileHeader.roomCount; i++)
	{
		chunks.push_back(Chunk());
		chunks[i].roomID = i;
	}

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
	
		glm::mat4* matrices = new glm::mat4[instanceCount];
	
		inFile.read((char*)matrices, sizeof(glm::mat4)*instanceCount);
		//meshes[i].setMatrices(matrices);

		for (int j = 0; j < instanceCount; j++)
		{
			chunks[roomIDs[j]].addProp(i, matrices[j]);
		}

		delete[] roomIDs;
		delete[] matrices;

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

		for (int j = 0; j < instanceCount; j++)
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

	for (int i = 0; i < pointLightCount; i++)
	{
		chunks[pl[i].roomID].addLight(*(SpotLightH*)&pl[i]);
	}
	delete[] pl;

	SpotLightH* sl = new SpotLightH[spotLightCount];
	inFile.read((char*)sl, sizeof(SpotLightH) * spotLightCount);
	delete[] sl;

	PortalDataRead* portalData = new PortalDataRead[portalCount];
	inFile.read((char*)portalData, sizeof(PortalDataRead) * portalCount);

	for (int p = 0; p < portalCount; p++)
	{
		if (portalData[p].bridgedRooms[0] == portalData[p].bridgedRooms[1])
		{
			std::cout << "Portal only in one room!\n";
		}
		else
		{
			chunks[portalData[p].bridgedRooms[0]].addPortal(portalData[p]);
			chunks[portalData[p].bridgedRooms[1]].addPortal(portalData[p]);
		}
	}

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