#ifndef MAPHEADERS_H
#define MAPHEADERS_H

#include <cstdlib>

struct SharedFileHDR
{
	uint32_t meshCount;
	uint32_t pointLightCount;
	uint32_t spotLightCount;
	uint32_t dirLightCount;
	uint32_t materialCount;
	uint32_t textureCount;
	uint32_t portalCount;
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
	float x, y, z;
	float r, g, b;
	float intensity;
	float ax, ay, az;
};

struct SpotLightH
{
	float x, y, z;
	float dx, dy, dz;
	float r, g, b;
	float intensity;
};

struct DirectionalLight
{
	float dx, dy, dz;
	float r, g, b;
	float intensity;
};

struct Material
{
	uint32_t textureMapIndex;
	uint32_t normalMapIndex;
	uint32_t specularMapIndex;
};

struct TextureHDR
{
	uint32_t* textureSize;
};

struct TextureData
{
	char* texturePath;
};

struct PortalData
{
	uint32_t portalID;
	uint32_t bridgedRooms[2];
	Vertex3 positions[4];
};

struct AAB
{
	float points[12];
};

struct BBPoint
{
	Vertex4 BBPos[8];
};


#endif