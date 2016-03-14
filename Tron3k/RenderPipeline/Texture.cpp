#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stbImage\stb_image.h"

#include <fstream>
#include <iostream>

using std::ios;

GLuint blank_diffuse;
GLuint blank_normal;
GLuint blank_glow;

bool hasEnding(std::string const &fullString, std::string const &ending) {
	if (fullString.length() >= ending.length()) {
		return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending));
	}
	else {
		return false;
	}
}

DDSTexture::~DDSTexture()
{
	if (textureData)
	{
		delete [] textureData;
	}
}

void DDSTexture::load(std::string file)
{

	std::ifstream inFile(file , ios::binary);

	width = 0;
	height = 0;

	if (inFile.is_open())
	{

		unsigned int magic;

		inFile.read((char*)&magic, sizeof(unsigned int));

		DDS_HEADER header;
		inFile.read((char*)&header, sizeof(header));

		if (header.ddspf.dwFourCC == MAKEFOURCC('D', 'X', 'T', '5'))
			format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
		else if (header.ddspf.dwFourCC == MAKEFOURCC('D', 'X', 'T', '4'))
			format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
		else if (header.ddspf.dwFourCC == MAKEFOURCC('D', 'X', 'T', '3'))
			format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
		else if (header.ddspf.dwFourCC == MAKEFOURCC('D', 'X', 'T', '2'))
			format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
		else if (header.ddspf.dwFourCC == MAKEFOURCC('D', 'X', 'T', '1'))
			format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
		else
		{
			printf("Rip\n");
			return;
		}


		width = header.dwWidth;
		height = header.dwHeight;

		blocksize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
		int factor = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 2 : 4;

		dataSize = ((width + 3) / 4) * ((height + 3) / 4) * (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT ? 8 : 16);

		if (dataSize == 0)
			return ;
		textureData = new char[dataSize];

		inFile.read((char*)textureData, dataSize);
		
	}
	else
	{
		printf("Failed loading texture %s\n", file.c_str());
	}

}

void* DDSTexture::getDataPtr(int &size, int &xres, int &yres, int &_format)
{
	char* ret = new char[dataSize];

	xres = width;
	yres = height;
	_format = format;

	if (textureData)
		memcpy(ret, textureData, dataSize);
	else
		ZeroMemory(ret, dataSize);
	size = dataSize;

	return ret;
}

bool DDSTexture::uploadTexture(GLuint &id)
{

	if (!textureData)
		return false;

	glBindTexture(GL_TEXTURE_2D, id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
	glCompressedTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, dataSize, textureData);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);

	return true;
}

void* loadTextureData(std::string texturePath, int &format, int &xres, int &yres, int &size)
{

	printf("LOADER: Loading texture %s\n", texturePath.c_str());

	int x;
	int y;
	int comp;

	GLuint textureID = 0;

	if (hasEnding(texturePath, ".dds") || hasEnding(texturePath, ".DDS"))
	{
		//printf("Loading dds texture %s\n", texturePath.c_str());

		DDSTexture tex;
		tex.load(texturePath);
		printf("LOADER: Texture size: %d\n", size);

		return tex.getDataPtr(size, xres, yres, format);

	}
	else
	{
		//printf("Loading other texture %s\n", texturePath.c_str());

		stbi_uc* image = stbi_load(texturePath.c_str(), &x, &y, &comp, 4);

		if (!(x > 0 && y > 0))
		{
			x = y = 1;
			comp = 4;
		}
		
		xres = x;
		yres = y;

		size = x*y*comp;

		char* data = new char[size];
		
		if (image)
			memcpy(data, image, size);
		else
			ZeroMemory(data, size);

		format = GL_COMPRESSED_RGBA;
		printf("LOADER: Texture %s size: %d\n", texturePath.c_str(), size);

		if(image)
			stbi_image_free(image);

		return data;
	}

}

GLuint loadTexture(std::string texturePath, bool PNGuploadComrpessint, int* xres, int* yres)
{
	int x;
	int y;
	int comp;
	printf("Loading texture %s\n", texturePath.c_str());
	stbi_uc* image = stbi_load(texturePath.c_str(), &x, &y, &comp, 4);

	if(xres)
		*xres = x;
	if(yres)
		*yres = y;

	GLuint textureID = 0;

	if (hasEnding(texturePath, ".dds") || hasEnding(texturePath, ".DDS"))
	{
		//printf("Loading dds texture %s\n", texturePath.c_str());

		glGenTextures(1, &textureID);
		DDSTexture tex;
		tex.load(texturePath);
		tex.uploadTexture(textureID);

	}
	else
	{
		//printf("Loading other texture %s\n", texturePath.c_str());

		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 1);

		if(x > 0 && y > 0)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, (void*)image);
		else
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
			printf("Failed loading texture data\n");
		}
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);

		stbi_image_free(image);
	}

	return textureID;

}