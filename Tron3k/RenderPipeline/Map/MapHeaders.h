#ifndef MAPHEADERS_H
#define MAPHEADERS_H

#include <cstdlib>

struct SharedFileHDR
{
	int64_t meshCount;
	int64_t pointLightCount;
	int64_t spotLightCount;
	int64_t dirLightCount;
	int64_t materialCount;
	int64_t textureCount;
	int64_t portalCount;
};

struct MeshDataHDR
{
	int32_t objectType;
	int32_t instanceCount;
	int32_t materialCount;
	int32_t indicesCount;
	int32_t vertexCount;
	int32_t bbCount;
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

struct MaterialHDR
{
	int64_t textureMapIndex;
	int64_t normalMapIndex;
	int64_t specularMapIndex;
};

struct TextureHDR
{
	int64_t* textureSize;
};

struct TextureData
{
	int64_t** texturePath;
};

struct PortalData
{
	int64_t portalID;
	int64_t bridgedRooms[2];
	float* positions[4];
};

#endif