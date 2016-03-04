#ifndef MAPHEADERS_H
#define MAPHEADERS_H

#include <cstdlib>
#include "../BlitQuad.h"
#include "../Mesh/StaticMesh.h"
#include "../Lights.h"
#include <vector>

#include "../ParticleSystem/ParticleSystem.h"

using std::vector;

struct SharedFileHDR
{
	uint32_t roomCount;
	uint32_t meshCount;
	uint32_t pointLightCount;
	uint32_t spotLightCount;
	uint32_t materialCount;
	uint32_t textureCount;
	uint32_t portalCount;
	uint32_t capturePointCount;
	uint32_t SPCountTeamA;
	uint32_t SPCountTeamB;
	uint32_t SPCountTeamFFA;
	uint32_t particleSystemCount;
};

struct CharacterHeader
{
	uint32_t materialCount;
	uint32_t textureCount;
	uint32_t indicesCount;
	uint32_t vertexCount;
	uint32_t animationCount;
	uint32_t jointCount;
};

struct CharacterHeaderV2
{
	uint32_t materialCount;
	uint32_t textureCount;
	uint32_t indicesCount;
	uint32_t vertexCount;
};

struct MeshDataHDR
{
	uint32_t objectType;
	uint32_t instanceCount;
	uint32_t materialCount;
	uint32_t indicesCount;
	uint32_t vertexCount;
	uint32_t bbCount;
};

struct PointLight
{
	int roomID;
	float r, g, b;
	float intensity;
	float x, y, z;
	float ambientIntensity;
	float dx, dy, dz;
	float coneAngle;
	float ax, ay, az, aw;
};

struct SpotLightH
{
	int roomID;
	float r, g, b;
	float intensity;
	float x, y, z;
	float ambientIntensity;
	float dx, dy, dz;
	float coneAngle;
	float ax, ay, az, aw;
};

struct Material
{
	int32_t materialID;
	int32_t textureMapIndex;
	int32_t normalMapIndex;
	int32_t specularMapIndex;
};

struct TextureHDR
{
	uint32_t textureSize;
};

struct TextureData
{
	char* texturePath;
};

struct TextureStruct
{
	std::string textureName;
	GLuint textureID;
	GLuint fileTexID;
};

struct PortalDataRead
{
	uint32_t portalID;
	uint32_t bridgedRooms[2];
	glm::vec4 positions[4];
	//glm::mat4 transform;
};

struct ParticleSystem_sdf
{
	glm::vec3 pos;
	int room;
	char* fileName;
};

struct PortalData
{
	GLuint query;
	GLuint available;
	GLuint passed;
	bool waiting;
	bool lastAvaliableState;
	bool rendered;

	uint32_t portalID;
	uint32_t bridgedRooms[2];
	glm::vec3 positions[4];
	glm::vec3 normal;

	//used for rendering the portal
	BlitQuad visualPortal;

	mat4 positionData;

	//calculated once, used for intesection test
	vec3 v1;
	vec3 v2;
	vec3 v3;
	vec3 v4;
	int tempCounter = 0;

	~PortalData()
	{
		glDeleteQueries(1, &query);
	}

	//init with bottom left and top right corners
	void init(uint32_t _portalID, uint32_t room1, uint32_t room2, vec3 topleft, vec3 topright, vec3 botright, vec3 botleft)
	{
		tempCounter = 0;
		query = 0;
		available = 0;
		passed = 0;
		waiting = false;
		lastAvaliableState = false;
		rendered = false;

		portalID = _portalID;
		bridgedRooms[0] = room1;
		bridgedRooms[1] = room2;

		positions[0] = topleft;
		positions[1] = topright;
		positions[2] = botright;
		positions[3] = botleft;

		positionData[0] = vec4(topleft, 1);
		positionData[1] = vec4(topright, 1);
		positionData[2] = vec4(botright, 1);
		positionData[3] = vec4(botleft, 1);
		
		positionData = transpose(positionData);

		v1 = normalize(positions[1] - positions[0]);
		v2 = normalize(positions[2] - positions[1]);
		v3 = normalize(positions[3] - positions[2]);
		v4 = normalize(positions[0] - positions[3]);

		normal = normalize(cross(v1, v4));

		visualPortal.Init(positions[0], positions[1], positions[2], positions[3]);
	}

	bool intersects(vec3 origin, vec3 dir, float len)
	{
		float denom = dot(normal, dir);

		if (length(denom) > 0.0001f)
		{
			float t = dot((positions[0] - origin), normal) / denom;

			if (t >= 0) //if we traveled away from the portal
			{
				if (len > t) //if we traveled far enough to cross the plane
				{
					//check if the intersection is within the portal
					vec3 inter = origin + t * dir;

					// XZ CHECK
		
					vec3 v5 = normalize(inter - positions[0]);
					vec3 v6 = normalize(inter - positions[2]);

					float test1 = dot(v1, v5);
					float test2 = dot(v3, v6);

					if (test1 > 0.0001 && test2 > 0.0001)
					{
						// Y check
						v5 = normalize(inter - positions[1]);
						v6 = normalize(inter - positions[3]);
						test1 = dot(v2, v5);
						test2 = dot(v4, v6);

						if (test1 > 0.0001 && test2 > 0.0001)
						{
							return true;
						}
					}
				}
			}
		}
		return false;
	}

	void render(GLuint shader, GLuint worldlocation)
	{
		tempCounter++;
		glProgramUniformMatrix4fv(shader, worldlocation, 1, GL_TRUE, &positionData[0][0]);
		glBeginQuery(GL_ANY_SAMPLES_PASSED, query);
		glDrawArrays(GL_POINTS, 0, 1);
		glEndQuery(GL_ANY_SAMPLES_PASSED);
	}

	bool passedCulling()
	{
		tempCounter++;

		if(tempCounter > 100)
		{
			glGetQueryObjectuiv(query, GL_QUERY_RESULT_AVAILABLE, &available);

			if (available)
			{
				waiting = false;
				glGetQueryObjectuiv(query, GL_QUERY_RESULT, &passed);
			
				if (passed)
				{
					tempCounter = 0;
					lastAvaliableState = true;
					return true;
				}
				else
				{
					lastAvaliableState = false;
					return false;
				}
			}
		}
		else
		{
			lastAvaliableState = true;
		}
		waiting = true;
		return lastAvaliableState;
	}
};

struct ABB
{
	float points[12];
};

struct BBPoint
{
	Vertex4 BBPos[8];
};

struct SpawnPoint
{
	uint32_t roomID;
	glm::mat4 transform;
	float dx, dy, dz;
};

struct Prop
{
	uint32_t id;
	vector<glm::mat4> mats;
};

struct OBB
{
	BBPoint point;
};

struct ABBFinishedCollision
{
	ABB abbBox;
	vector<OBB> obbBoxes;
};

struct TriangleVertex
{
	float data[5]; //pos, uv
};

struct renderDataCube
{
	GLuint vertexDataId;
	GLuint gVertexAttribute;

	void init_ABB(ABB abb)
	{
		vec3 corner0 = vec3(abb.points[8], abb.points[9], abb.points[10]);
		vec3 corner5 = vec3(abb.points[4], abb.points[5], abb.points[6]);
		
		vec3 corner1 = vec3(corner5.x, corner0.y, corner0.z);
		vec3 corner6 = vec3(corner0.x, corner0.y, corner5.z);
		vec3 corner7 = vec3(corner5.x, corner0.y, corner5.z);
		
		vec3 corner2 = vec3(corner0.x, corner5.y, corner0.z);
		vec3 corner3 = vec3(corner5.x, corner5.y, corner0.z);
		vec3 corner4 = vec3(corner0.x, corner5.y, corner5.z);

		TriangleVertex* vex = new TriangleVertex[20];

		vec3 a;							
		a = corner0;	vex[0] = { a.x, a.y, a.z, 0.0f, 0.0f };
		a = corner1;	vex[1] = { a.x, a.y, a.z, 0.0f, 0.0f };
		a = corner2;	vex[2] = { a.x, a.y, a.z, 0.0f, 0.0f };
		a = corner3;	vex[3] = { a.x, a.y, a.z, 0.0f, 0.0f };
		a = corner4;	vex[4] = { a.x, a.y, a.z, 0.0f, 0.0f };
		a = corner5;	vex[5] = { a.x, a.y, a.z, 0.0f, 0.0f };
		a = corner6;	vex[6] = { a.x, a.y, a.z, 0.0f, 0.0f };
		a = corner7;	vex[7] = { a.x, a.y, a.z, 0.0f, 0.0f };
		a = corner0;	vex[8] = { a.x, a.y, a.z, 0.0f, 0.0f };
		a = corner1;	vex[9] = { a.x, a.y, a.z, 0.0f, 0.0f };

		a = corner1;	vex[10] = { a.x, a.y, a.z, 0.0f, 0.0f };
		a = corner7;	vex[11] = { a.x, a.y, a.z, 0.0f, 0.0f };
		a = corner3;	vex[12] = { a.x, a.y, a.z, 0.0f, 0.0f };
		a = corner5;	vex[13] = { a.x, a.y, a.z, 0.0f, 0.0f };

		a = corner5;	vex[14] = { a.x, a.y, a.z, 0.0f, 0.0f };
		a = corner4;	vex[15] = { a.x, a.y, a.z, 0.0f, 0.0f };
		a = corner4;	vex[16] = { a.x, a.y, a.z, 0.0f, 0.0f };
		a = corner2;	vex[17] = { a.x, a.y, a.z, 0.0f, 0.0f };
		a = corner6;	vex[18] = { a.x, a.y, a.z, 0.0f, 0.0f };
		a = corner0;	vex[19] = { a.x, a.y, a.z, 0.0f, 0.0f };

		createData(vex);
	}

	void init_OBB(OBB obb)
	{
		TriangleVertex* vex = new TriangleVertex[20];

		Vertex4 v;
		v = obb.point.BBPos[0];		vex[0] = { v.x, v.y, v.z, 0.0f, 0.0f };
		v = obb.point.BBPos[1];		vex[1] = { v.x, v.y, v.z, 0.0f, 0.0f };
		v = obb.point.BBPos[2];		vex[2] = { v.x, v.y, v.z, 0.0f, 0.0f };
		v = obb.point.BBPos[3];		vex[3] = { v.x, v.y, v.z, 0.0f, 0.0f };
		v = obb.point.BBPos[4];		vex[4] = { v.x, v.y, v.z, 0.0f, 0.0f };
		v = obb.point.BBPos[5];		vex[5] = { v.x, v.y, v.z, 0.0f, 0.0f };
		v = obb.point.BBPos[6];		vex[6] = { v.x, v.y, v.z, 0.0f, 0.0f };
		v = obb.point.BBPos[7];		vex[7] = { v.x, v.y, v.z, 0.0f, 0.0f };
		v = obb.point.BBPos[0];		vex[8] = { v.x, v.y, v.z, 0.0f, 0.0f };
		v = obb.point.BBPos[1];		vex[9] = { v.x, v.y, v.z, 0.0f, 0.0f };
		v = obb.point.BBPos[1];		vex[10] = { v.x, v.y, v.z, 0.0f, 0.0f };
		v = obb.point.BBPos[7];		vex[11] = { v.x, v.y, v.z, 0.0f, 0.0f };
		v = obb.point.BBPos[3];		vex[12] = { v.x, v.y, v.z, 0.0f, 0.0f };
		v = obb.point.BBPos[5];		vex[13] = { v.x, v.y, v.z, 0.0f, 0.0f };
		v = obb.point.BBPos[5];		vex[14] = { v.x, v.y, v.z, 0.0f, 0.0f };
		v = obb.point.BBPos[4];		vex[15] = { v.x, v.y, v.z, 0.0f, 0.0f };
		v = obb.point.BBPos[4];		vex[16] = { v.x, v.y, v.z, 0.0f, 0.0f };
		v = obb.point.BBPos[2];		vex[17] = { v.x, v.y, v.z, 0.0f, 0.0f };
		v = obb.point.BBPos[6];		vex[18] = { v.x, v.y, v.z, 0.0f, 0.0f };
		v = obb.point.BBPos[0];		vex[19] = { v.x, v.y, v.z, 0.0f, 0.0f };

		createData(vex);
	}

	void createData(TriangleVertex* vex)
	{
		glGenBuffers(1, &vertexDataId);
		glBindBuffer(GL_ARRAY_BUFFER, vertexDataId);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vex[0]) * 20, &vex[0], GL_STATIC_DRAW);
		delete[] vex;

		//define vertex data layout
		glGenVertexArrays(1, &gVertexAttribute);
		glBindVertexArray(gVertexAttribute);
		glEnableVertexAttribArray(0); //the vertex attribute object will remember its enabled attributes
		glEnableVertexAttribArray(1);
		//pos
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(TriangleVertex), BUFFER_OFFSET(0));
		//uv
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(TriangleVertex), BUFFER_OFFSET(sizeof(float) * 3));
	}

	void BindVertData()
	{
		glBindBuffer(GL_ARRAY_BUFFER, vertexDataId);
		glBindVertexArray(gVertexAttribute);
	}

	void release()
	{
		glDeleteBuffers(1, &vertexDataId);
		glDeleteVertexArrays(1, &gVertexAttribute);
	}
};

struct CollisionRenderABB
{
	renderDataCube abbBoxR;
	vector<renderDataCube> obbBoxesR;

	void in_data(ABBFinishedCollision in)
	{
		abbBoxR.init_ABB(in.abbBox);
		int nrOBBs = in.obbBoxes.size();

		for (int n = 0; n < nrOBBs; n++)
		{
			obbBoxesR.push_back(renderDataCube());
			obbBoxesR[n].init_OBB(in.obbBoxes[n]);
		}
	}
};

struct ChunkCollisionRender
{
	void add(ABBFinishedCollision in)
	{
		abbRender.push_back(CollisionRenderABB()); //creates a new entry
		abbRender[abbRender.size() - 1].in_data(in); //fill it with information
	}
	vector<CollisionRenderABB> abbRender;
};

struct ChunkCollision
{
	vector<ABBFinishedCollision> abbStuff;
};

struct Chunk
{
	Chunk()
	{
		roomID = -1;
		color = { 1, 1, 1 };
		staticIntes = 1;
		nrStaticSpotlights = 0;
		nrStaticPointlights = 0;
	}

	void addProp(int id, glm::mat4 matrix)
	{
		for (size_t i = 0; i < props.size(); i++)
		{
			if (props[i].id == id)
			{
				props[i].mats.push_back(matrix);
				return;
			}
		}
		props.push_back(Prop());
		props[props.size() - 1].id = id;
		props[props.size() - 1].mats.push_back(matrix);
	}

	void addPortal(PortalDataRead port)
	{
		PortalData pd;

		pd.positions[0] = glm::vec3(port.positions[0]);
		pd.positions[1] = glm::vec3(port.positions[1]);
		pd.positions[2] = glm::vec3(port.positions[2]);
		pd.positions[3] = glm::vec3(port.positions[3]);

		pd.init(port.portalID, port.bridgedRooms[0], port.bridgedRooms[1], glm::vec3(port.positions[0]), glm::vec3(port.positions[2]), glm::vec3(port.positions[3]), glm::vec3(port.positions[1]));

		portals.push_back(pd);
	}

	void addLight(SpotLightH light)
	{
		SpotLight l;
		l.Color = vec3(light.r, light.g, light.b);
		l.DiffuseIntensity = light.intensity;
		l.Position = vec3(light.x, light.y, light.z);
		l.AmbientIntensity = light.ambientIntensity;
		l.Direction = vec3(light.dx, light.dy, light.dz);
		if (length(l.Direction) > 0.001f)
		{
			nrStaticSpotlights++;
			l.Direction = normalize(l.Direction);
		}
		else
			nrStaticPointlights++;

		l.Cutoff = light.coneAngle;
		l.attenuation = vec4(light.ax, light.ay, light.az, light.aw);

		lights.push_back(l);
	}

	void addCollisionMesh(ABBFinishedCollision collision)
	{
		collisionMesh.abbStuff.push_back(collision);
		collisionRender.add(collision);
	}

	void addRoomBoxRender(ABB in)
	{
		ABBFinishedCollision temp;
		temp.abbBox = in;
		temp.obbBoxes.clear();
		roomOnlyRender.add(temp);
	}

	ChunkCollision* getChunkCollision()
	{
		return &collisionMesh;
	}

	glm::vec3 color;
	float staticIntes;

	uint32_t roomID;

	vector<Prop> props;

	vector<PortalData> portals;

	vector<SpotLight> lights;

	vector<ParticleSystem> particleSystem;

	int nrStaticSpotlights;
	int nrStaticPointlights;


	ChunkCollision collisionMesh;

	ChunkCollisionRender collisionRender;
	ChunkCollisionRender roomOnlyRender;

	ABB roomBox;

};
#endif