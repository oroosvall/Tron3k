#ifndef MAP_H
#define MAP_H

#include "../StaticMesh.h"
#include "MapHeaders.h"

#include <string>

class Map
{

private:

	StaticMesh* meshes;

public:
	
	void init();
	void release();

	void loadMap(std::string mapName);

};

#endif