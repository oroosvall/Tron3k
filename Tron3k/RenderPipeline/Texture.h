#ifndef TEXTURE_H
#define TEXTURE_H

#include "Utils\GPUMemoryLeakChecker.h"
#include <string>
#include <cstdlib>

#include <dxgi.h>

extern GLuint blank_diffuse;
extern GLuint blank_normal;
extern GLuint blank_glow;

#define DDS_MAGIC 0x20534444 // "DDS "

struct DDS_PIXELFORMAT {
	unsigned long dwSize;
	unsigned long dwFlags;
	unsigned long dwFourCC;
	unsigned long dwRGBBitCount;
	unsigned long dwRBitMask;
	unsigned long dwGBitMask;
	unsigned long dwBBitMask;
	unsigned long dwABitMask;
};

typedef struct {
	unsigned long dwSize;
	unsigned long dwFlags;
	unsigned long dwHeight;
	unsigned long dwWidth;
	unsigned long dwPitchOrLinearSize;
	unsigned long dwDepth;
	unsigned long dwMipMapCount;
	unsigned long dwReserved1[11];
	DDS_PIXELFORMAT ddspf;
	unsigned long dwCaps;
	unsigned long dwCaps2;
	unsigned long dwCaps3;
	unsigned long dwCaps4;
	unsigned long dwReserved2;
} DDS_HEADER;

enum TEXTURE_RESOURCE_DIMENSION
{
	TEXTURE_RESOURCE_DIMENSION_UNKNOWN		= 0,
	TEXTURE_RESOURCE_DIMENSION_BUFFER		= 1,
	TEXTURE_RESOURCE_DIMENSION_TEXTURE1D	= 2,
	TEXTURE_RESOURCE_DIMENSION_TEXTURE2D	= 3,
	TEXTURE_RESOURCE_DIMENSION_TEXTURE3D	= 4,
};

typedef struct {
	DXGI_FORMAT						dxgiFormat;
	TEXTURE_RESOURCE_DIMENSION		resourceDimension;
	unsigned long                   miscFlag;
	unsigned long                   arraySize;
	unsigned long                   miscFlags2;
} DDS_HEADER_DXT10;

class DDSTexture
{
private:

public:

	~DDSTexture();

	int width;
	int height;

	int blocksize;

	int format;

	unsigned int dataSize;
	void* textureData;
	
	void* getDataPtr(int &size, int &xres, int &yres, int &format);

	void load(std::string texturePath);

	bool uploadTexture(GLuint &id);

};

void* loadTextureData(std::string texturePath, int &format, int &xres, int &yres, int &size);

GLuint loadTexture(std::string texturePath, bool PNGUploadCompress = true, int* xres = 0, int* yres = 0);

#endif