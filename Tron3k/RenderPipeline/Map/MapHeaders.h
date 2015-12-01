#ifndef MAPHEADERS_H
#define MAPHEADERS_H

#include <cstdlib>
#include "../BlitQuad.h"
#include "../StaticMesh.h"

struct SharedFileHDR
{
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
	uint32_t materialID;
	uint32_t textureMapIndex;
	uint32_t normalMapIndex;
	uint32_t specularMapIndex;
};

struct TextureHDR
{
	uint32_t textureSize;
};

struct TextureData
{
	char* texturePath;
};

struct PortalDataRead
{
	uint32_t portalID;
	uint32_t bridgedRooms[2];
	glm::vec4 positions[4];
	//glm::mat4 transform;
};

struct PortalData
{
	uint32_t portalID;
	uint32_t bridgedRooms[2];
	glm::vec3 positions[4];
	glm::vec3 normal;

	//used for rendering the portal
	BlitQuad visualPortal;

	//calculated once, used for intesection test
	vec3 v1;
	vec3 v2;
	vec3 v3;
	vec3 v4;

	//init with bottom left and top right corners
	void init(uint32_t _portalID, uint32_t room1, uint32_t room2, vec3 topleft, vec3 topright, vec3 botright, vec3 botleft)
	{
		portalID = _portalID;
		bridgedRooms[0] = room1;
		bridgedRooms[1] = room2;

		positions[0] = topleft;
		positions[1] = topright;
		positions[2] = botright;
		positions[3] = botleft;

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

	void render()
	{
		visualPortal.BindVertData();
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}
};

struct AAB
{
	float points[12];
};

struct BBPoint
{
	Vertex4 BBPos[8];
};

struct CapturePoints
{
	uint32_t capturePoints;
};

struct SpawnPoint
{
	uint32_t roomID;
	glm::mat4 transform;
	float dx, dy, dz;
};

struct Chunk
{
	uint32_t roomID;

	int meshCount;
	StaticMesh* meshes;

	int nrPortals;
	PortalData* portals;

	int nrLights;
	SpotLightH* lights;

};
#endif