#include "Map.h"
#include <fstream>
#include <iostream>

void Map::init()
{
	currentChunk = -1;

	loadMap("GameFiles/TestFiles/Tron3k_map_1_textures.bin");

	for (int i = 0; i < meshCount; i++)
	{
		meshes[i].stream();
	}
	for (int i = 0; i < textureCount; i++)
	{
		tex[i].textureID = loadTexture("GameFiles/testfiles/" + std::string(tex[i].textureName));
	}


	//init test portal
								//     1 --------- 2
	p.portalID = 0;				//	   |           |
	p.bridgedRooms[0] = 1;		//	   |           |
	p.bridgedRooms[1] = 2;		//     4 --------- 3

#define pp p.positions

	//1
	pp[0].x = 5;
	pp[0].y = -10;
	pp[0].z = 10;
	//2
	pp[1].x = 10;
	pp[1].y = -10;
	pp[1].z = 10;
	//3
	pp[2].x = 10;
	pp[2].y = -15;
	pp[2].z = 10;
	//4
	pp[3].x = 5;
	pp[3].y = -15;
	pp[3].z = 10;

	//get two vectors
	vec3 _vec1 = pp[1] - pp[0];
	vec3 _vec2 = pp[3] - pp[0];
	p.normal = cross(_vec1, _vec2);

	p.A = p.normal.x;
	p.B = p.normal.y;
	p.C = p.normal.z;

	p.normal = normalize(p.normal);

	p.D = p.A * pp[0].x + p.B * pp[0].y + p.C * pp[0].z;

	//to draw, setup the drawing object
	portal.Init(p.positions[3], p.positions[1]);

#undef pp

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
	int dirLightCount = (int)fileHeader.dirLightCount;
	int materialCount = (int)fileHeader.materialCount;
	textureCount = (int)fileHeader.textureCount;
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
	
	PointLight* pl = new PointLight[pointLightCount];
	inFile.read((char*)pl, sizeof(PointLight) * pointLightCount);
	delete[] pl;
	
	SpotLightH* sl = new SpotLightH[spotLightCount];
	inFile.read((char*)sl, sizeof(SpotLightH) * spotLightCount);
	delete[] sl;
	
	DirectionalLight* dl = new DirectionalLight[dirLightCount];
	inFile.read((char*)dl, sizeof(DirectionalLight) * dirLightCount);
	delete[] dl;
	
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
	
	PortalData* portalData = new PortalData[portalCount];
	inFile.read((char*)portalData, sizeof(PortalData) * portalCount);
	delete[] portalData;
	
	inFile.close();

}

int Map::getChunkID(glm::vec3 oldPos, glm::vec3 newPos)
{
	//intersection test vs all portals in this room

	//Extract Plane formula http://answers.google.com/answers/threadview?id=18979

#define pp p.positions

	//Ray origin = oldPos
	//Ray direction = Rd
	glm::vec3 rd = newPos - oldPos;
	float len = length(rd);
	if (len == 0) // we didnt move
		return currentChunk;
	rd = glm::normalize(rd); // not sure if i should notmalize or not


	float denom = dot(p.normal, rd);

	if (length(denom) > 0.0001f) 
	{
		float t = dot((p.positions[0] - oldPos), p.normal) / denom;
		if (t >= 0) 
		{
			if (length(newPos - oldPos) > t) //went though plane
			{
				//check if the value is within the portal
				vec3 inter = oldPos + t * rd;

				if (inter.y < p.positions[0].y && inter.y > p.positions[3].y)
				{
					vec3 _v1 = p.positions[1] - p.positions[0]; //static data 
					vec3 _v3 = p.positions[3] - p.positions[2]; //static data
					vec3 _v4 = inter - p.positions[0];
					vec3 _v5 = inter - p.positions[2];

					_v1 = normalize(_v1); //static data 
					_v3 = normalize(_v3); //static data 
					_v4 = normalize(_v4);
					_v5 = normalize(_v5);

					float test1 = dot(_v1, _v4);
					float test2 = dot(_v3, _v5);

					if(test1 > 0.0001 && test2 > 0.0001)
						printf("went though portal \n");
				}
			}
		}
	}

#undef pp

	return currentChunk;






	//intersection point  R(t) = (x0 + xd*t, y0 + yd*t, z0 + zd*t).
	//glm::vec3 interPos = glm::vec3(oldPos.x * rd.x * t, oldPos.y * rd.y * t, oldPos.z * rd.z * t);
	//glm::vec3 V1 = pp[1] - pp[0]; 
	//glm::vec3 V3 = pp[3] - pp[2]; 
	//glm::vec3 V4 = interPos - pp[0]; // 1st corenr to interpoint
	//glm::vec3 V5 = interPos - pp[2]; // 3rd corner to interpoint
	//glm::normalize(V1);
	//glm::normalize(V3);
	//glm::normalize(V4);
	//glm::normalize(V5);
	////V1 dot V4 and V3 dot V5. If both are non - negative, then the point is in the rectangle.
	//float test1 = dot(V1, V4);
	//float test2 = dot(V3, V5);
	//
	//if (test1 > 0 && test2 > 0)
	//	int k = 222;


}

void Map::renderPortals()
{
	portal.BindVertData();
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}