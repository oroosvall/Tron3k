#include "Map.h"
#include <fstream>
#include <iostream>

void CapturePoint::buildABBS()
{
	renderBigAbb.abbBoxR.init_ABB(bigAABB);

	renderAbb = new CollisionRenderABB[aabbCount];

	for (int i = 0; i < aabbCount; i++)
	{
		renderAbb[i].abbBoxR.init_ABB(aabb[i]);
	}
}

void CapturePoint::release()
{

	renderBigAbb.abbBoxR.release();

	for (int i = 0; i < aabbCount; i++)
	{
		renderAbb[i].abbBoxR.release();
	}

	delete[] renderAbb;

	delete aabb;

	delete[] mats;

	for (int i = 0; i < meshCount; i++)
	{
		meshes[i].release();
	}

	delete[] meshes;

}

void Map::init()
{
	currentChunk = 0;
	spA = 0;
	spB = 0;
	spFFA = 0;

	//loadMap("GameFiles/TestFiles/PortalTest.bin");
	loadMap("GameFiles/TestFiles/tron3k_map.bin");
	for (int i = 0; i < meshCount; i++)
	{
		meshes[i].stream();
	}
	for (int i = 0; i < textureCount; i++)
	{
		tex[i].textureID = loadTexture("GameFiles/Textures/map/" + std::string(tex[i].textureName));
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

	if (materials)
	{
		delete[] materials;
		materials = nullptr;
	}

	if(chunkAABB)
		delete[] chunkAABB;

	if (tex)
	{
		for (int i = 0; i < textureCount; i++)
		{
			glDeleteTextures(1, &tex[i].textureID);
		}
		delete[] tex;
		tex = nullptr;
	}

	for (size_t i = 0; i < chunks.size(); i++)
	{
		for (size_t p = 0; p < chunks[i].portals.size(); p++)
		{
			chunks[i].portals[p].visualPortal.release();
		}
	}

	if (capturePoints)
	{
		for (int i = 0; i < capCount; i++)
		{
			capturePoints[i].release();
		}
		delete[] capturePoints;
	}

	if (spA)
		delete[] spA;
	if (spB)
		delete[] spB;
	if (spFFA)
		delete[] spFFA;

	//if (bbPoints)
	//{
	//	delete[] bbPoints;
	//	bbPoints = nullptr;
	//}

}

void Map::renderChunk(GLuint shader, GLuint shaderLocation, int chunkID)
{
	int meshID;

	Material mat = {-1, -1, -1 ,-1};

	for (size_t i = 0; i < chunks[chunkID].props.size(); i++)
	{
		meshID = chunks[chunkID].props[i].id;

		for (size_t i = 0; i < materialCount; i++)
		{
			if (meshes[meshID].material == materials[i].materialID)
			{
				mat = materials[i];
				break;
			}
		}

		glActiveTexture(GL_TEXTURE0);
		if(mat.textureMapIndex != -1)
			glBindTexture(GL_TEXTURE_2D, tex[materials[meshes[meshID].material].textureMapIndex].textureID);
		else
			glBindTexture(GL_TEXTURE_2D, blank_diffuse);
		glActiveTexture(GL_TEXTURE0 + 1);
		if (mat.normalMapIndex != -1)
			glBindTexture(GL_TEXTURE_2D, tex[materials[meshes[meshID].material].normalMapIndex].textureID);
		else
			glBindTexture(GL_TEXTURE_2D, blank_normal);
		glActiveTexture(GL_TEXTURE0 + 2);
		if (mat.specularMapIndex != -1)
			glBindTexture(GL_TEXTURE_2D, tex[materials[meshes[meshID].material].specularMapIndex].textureID);
		else
			glBindTexture(GL_TEXTURE_2D, blank_glow);
		
		glBindVertexArray(meshes[meshID].vertexArray);
		glBindBuffer(GL_ARRAY_BUFFER, meshes[meshID].vertexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshes[meshID].indexBuffer);

		for (size_t ins = 0; ins < chunks[chunkID].props[i].mats.size(); ins++)
		{
			glProgramUniformMatrix4fv(shader, shaderLocation, 1, GL_FALSE, (GLfloat*)&chunks[chunkID].props[i].mats[ins][0][0]);

			glDrawElements(GL_TRIANGLES, meshes[meshID].indexCount, GL_UNSIGNED_INT, 0);
		}
	}

	if (capturePoints[0].roomID == chunkID)
	{
		renderCapturePoint(shader, shaderLocation, 0);
	}

}

void Map::renderCapturePoint(GLuint shader, GLuint shaderLocation, int capturePointID)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, blank_diffuse);
	glBindVertexArray(capturePoints[capturePointID].meshes[0].vertexArray);
	glBindBuffer(GL_ARRAY_BUFFER, capturePoints[capturePointID].meshes[0].vertexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, capturePoints[capturePointID].meshes[0].indexBuffer);

	glProgramUniformMatrix4fv(shader, shaderLocation, 1, GL_FALSE, (GLfloat*)&capturePoints[0].mats[0][0][0]);

	glDrawElements(GL_TRIANGLES, capturePoints[capturePointID].meshes[0].indexCount, GL_UNSIGNED_INT, 0);
}

void Map::renderCapAbb()
{
	capturePoints[0].renderBigAbb.abbBoxR.BindVertData();
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 20);

	for (int i = 0; i < capturePoints[0].aabbCount; i++)
	{
		capturePoints[0].renderAbb[i].abbBoxR.BindVertData();
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 20);
	}
}

void Map::loadMap(std::string mapName)
{
	using namespace std;

	ifstream inFile;
	inFile.open(mapName, ios::in | ios::binary);
	
	if (!inFile.is_open())
		return;
	SharedFileHDR fileHeader;
	
	inFile.read((char*)&fileHeader, sizeof(fileHeader));
	
	cout << sizeof(fileHeader) << endl;
	
	roomCount = (int)fileHeader.roomCount;
	meshCount = (int)fileHeader.meshCount;
	int pointLightCount = (int)fileHeader.pointLightCount;
	int spotLightCount = (int)fileHeader.spotLightCount;
	materialCount = (int)fileHeader.materialCount;
	textureCount = (int)fileHeader.textureCount;
	int portalCount = (int)fileHeader.portalCount;
	capCount = (int)fileHeader.capturePointCount;
	spTACount = (int)fileHeader.SPCountTeamA;
	spTBCount = (int)fileHeader.SPCountTeamB;
	spFFACount = (int)fileHeader.SPCountTeamFFA;
	
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
	
		ABB* bounds = new ABB[instanceCount];
		inFile.read((char*)bounds, sizeof(ABB)*instanceCount);
		
		// temp fix for leaks
		
		for (int j = 0; j < instanceCount; j++)
		{
			bbPoints = new BBPoint[bbCount];
			inFile.read((char*)bbPoints, sizeof(BBPoint)*bbCount);

			ABBFinishedCollision abbBox;
			abbBox.abbBox = bounds[j];

			for (int b = 0; b < bbCount; b++)
			{
				OBB obbBox;
				obbBox.point = bbPoints[b];
				abbBox.obbBoxes.push_back(obbBox);
			}

			chunks[roomIDs[j]].addCollisionMesh(abbBox);

			delete[] bbPoints;
		}

		delete[] bounds;
		delete[] roomIDs;
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
	
	for (int i = 0; i < spotLightCount; i++)
	{
		chunks[sl[i].roomID].addLight(sl[i]);
	}
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
	
	capPointAABBCount = new int[capCount];
	capPointWallCount = new int[capCount];

	inFile.read((char*)capPointAABBCount, sizeof(int) * capCount);
	inFile.read((char*)capPointWallCount, sizeof(int) * capCount);

	capturePoints = new CapturePoint[capCount];

	for (int i = 0; i < capCount; i++)
	{
		int room;
		ABB bigAABB;
		ABB* aabbs = new ABB[capPointAABBCount[i]];

		inFile.read((char*)&room, sizeof(int));
		inFile.read((char*)&bigAABB, sizeof(ABB));
		inFile.read((char*)aabbs, sizeof(ABB) * capPointAABBCount[i]);

		capturePoints[i].roomID = room;
		capturePoints[i].bigAABB = bigAABB;
		capturePoints[i].aabb = aabbs;

		int* indicesCount = new int[capPointWallCount[i]];
		int* vertexCount = new int[capPointWallCount[i]];
		
		inFile.read((char*)indicesCount, sizeof(int) * capPointWallCount[i]);
		inFile.read((char*)vertexCount, sizeof(int) * capPointWallCount[i]);

		capturePoints[i].meshes = new StaticMesh[capPointWallCount[i]];
		capturePoints[i].mats = new glm::mat4[capPointWallCount[i]];

		capturePoints[i].meshCount = capPointWallCount[i];
		capturePoints[i].aabbCount = capPointAABBCount[i];

		for (int j = 0; j < capPointWallCount[i]; j++)
		{
			glm::mat4 mat;

			int* indices = new int[indicesCount[j]];
			Vertex11* verts = new Vertex11[vertexCount[j]];

			inFile.read((char*)&mat, sizeof(glm::mat4));

			inFile.read((char*)indices, sizeof(int) * indicesCount[j]);
			inFile.read((char*)verts, sizeof(Vertex11) * vertexCount[j]);

			capturePoints[i].mats[j] = mat;

			capturePoints[i].meshes[j].init(1);
			capturePoints[i].meshes[j].setVertices((float*&)verts, vertexCount[j]);
			capturePoints[i].meshes[j].setIndices((int*&)indices, indicesCount[j]);
			capturePoints[i].meshes[j].stream();

		}

		capturePoints[i].buildABBS();

		delete[] indicesCount;
		delete[] vertexCount;

	}

	delete[] capPointAABBCount;
	delete[] capPointWallCount;
	
	spA = new SpawnPoint[spTACount];
	spB = new SpawnPoint[spTBCount];
	spFFA = new SpawnPoint[spFFACount];
	
	inFile.read((char*)spA, sizeof(SpawnPoint) * spTACount);
	inFile.read((char*)spB, sizeof(SpawnPoint) * spTBCount);
	inFile.read((char*)spFFA, sizeof(SpawnPoint) * spFFACount);


	chunkAABB = new ABB[roomCount];

	inFile.read((char*)chunkAABB, sizeof(ABB) * (roomCount));

	for (int i = 0; i < roomCount; i++)
	{
		chunks[i].roomBox = chunkAABB[i];
	}

	inFile.close();

}

int Map::portalintersection(glm::vec3* oldPos, glm::vec3* newPos, int in_currentChunk)
{
	vec3 dir = *newPos - *oldPos;
	float len = length(dir);
	dir = normalize(dir);

	for (size_t n = 0; n < chunks[in_currentChunk].portals.size(); n++)
		if (chunks[in_currentChunk].portals[n].intersects(*oldPos, dir, len))
		{
			if (chunks[in_currentChunk].portals[n].bridgedRooms[0] == in_currentChunk)
				in_currentChunk = chunks[in_currentChunk].portals[n].bridgedRooms[1];
			else
				in_currentChunk = chunks[in_currentChunk].portals[n].bridgedRooms[0];
			return in_currentChunk;
		}
	
	//no intersection
	return -1;
}

ChunkCollision* Map::getChunkCollision(int chunkID)
{
	return chunks[chunkID].getChunkCollision();
}

void* Map::getCapAsPointer(int& count)
{
	count = capCount;

	CaptureExportToGame* cap = new CaptureExportToGame[capCount];

	for (int n = 0; n < capCount; n++)
	{
		cap[n].roomID = capturePoints[n].roomID;
		cap[n].bigAABB = capturePoints[n].bigAABB;
		cap[n].subcount = capturePoints[n].aabbCount;
		cap[n].subabbs = capturePoints[n].aabb;
	}
	
	return cap;
}

void Map::deleteSpawnposData()
{
	if (spA)
		delete[] spA;
	if (spB)
		delete[] spB;
	if (spFFA)
		delete[] spFFA;

	spA = nullptr;
	spB = nullptr;
	spFFA = nullptr;
}