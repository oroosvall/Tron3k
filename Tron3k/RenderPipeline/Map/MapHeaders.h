#ifndef MAPHEADERS_H
#define MAPHEADERS_H

#include <cstdlib>
#include "../BlitQuad.h"
#include "../StaticMesh.h"
#include "../Lights.h"
#include <vector>

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

struct Prop
{
	uint32_t id;
	vector<glm::mat4> mats;
};

struct Chunk
{
	Chunk()
	{
		roomID = -1;
		color = { 1, 1, 1 };
		staticIntes = 1;
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
		if (length(l.Direction) <= 0.001f)
			l.Direction.y = -1;
		l.Cutoff = light.coneAngle;
		l.Cutoff = 0.7f;

		lights.push_back(l);
	}

	glm::vec3 color;
	float staticIntes;

	uint32_t roomID;

	vector<Prop> props;

	vector<PortalData> portals;

	vector<SpotLight> lights;

};
#endif