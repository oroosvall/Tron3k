#ifndef MAPHEADERS_H
#define MAPHEADERS_H

struct MharedFileHDR
{
	int meshCount;
	int pointLightCount;
	int spotLightCount;
	int dirLightCount;
	int materialCount;
	int textureCount;
	int portalCount;
};

struct MeshDataHDR
{
	int objectType;
	int instanceCount;
	int materialCount;
	int indicesCount;
	int vertexCount;
	int bbCount;
};

struct PointLight
{
	float x, y, z;
	float r, g, b;
	float intensity;
	float ax, ay, az;
};

struct SpotLight
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
	int textureMapIndex;
	int normalMapIndex;
	int specularMapIndex;
};

struct TextureHDR
{
	int* textureSize;
};

struct TextureData
{
	int** texturePath;
};

struct PortalData
{
	int portalID;
	int bridgedRooms[2];
	float* positions[4];
};

#endif