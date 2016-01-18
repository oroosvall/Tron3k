#ifndef TEXTURE_H
#define TEXTURE_H

#include "Utils\GPUMemoryLeakChecker.h"
#include <string>

extern GLuint blank_diffuse;
extern GLuint blank_normal;
extern GLuint blank_glow;

GLuint loadTexture(std::string texturePath);

#endif