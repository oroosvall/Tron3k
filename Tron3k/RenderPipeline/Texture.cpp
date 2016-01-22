#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stbImage\stb_image.h"

#include <fstream>

GLuint blank_diffuse;
GLuint blank_normal;
GLuint blank_glow;

void DDSTexture::load(std::string file)
{

	std::ifstream inFile(file);

	width = 0;
	height = 0;

	if (inFile.is_open())
	{

		unsigned int magic;

		inFile.read((char*)&magic, sizeof(unsigned int));

		DDS_HEADER header;
		inFile.read((char*)&header, sizeof(header));

		if (header.ddspf.dwFourCC != MAKEFOURCC('D', 'X', 'T', '5'))
		{
			return;
		}
		else
		{

		}
	}

}

bool DDSTexture::uploadTexture(GLuint &id)
{

	return true;
}

GLuint loadTexture(std::string texturePath)
{
	int x;
	int y;
	int comp;
	
	stbi_uc* image = stbi_load(texturePath.c_str(), &x, &y, &comp, 4);

	GLuint textureID = 0;

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, (void*)image);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);

	stbi_image_free(image);

	return textureID;

}