#ifndef TEXTURE_H
#define TEXTURE_H

#include "Utils\GPUMemoryLeakChecker.h"
#include <string>

static GLuint blank_diffuse;
static GLuint blank_normal;
static GLuint blank_glow;

GLuint loadTexture(std::string texturePath);

#endif