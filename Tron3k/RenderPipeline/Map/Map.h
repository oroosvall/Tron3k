#ifndef MAP_H
#define MAP_H

#include "../StaticMesh.h"
#include "MapHeaders.h"

#include <string>

class Map
{

private:

	StaticMesh* meshes;

	int meshCount;

public:
	
	void init();
	void release();

	void render(GLuint shader, GLuint shaderLocation);

	void loadMap(std::string mapName);

};

#endif