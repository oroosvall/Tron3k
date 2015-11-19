#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stbImage\stb_image.h"

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
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);

	stbi_image_free(image);

	return textureID;

}