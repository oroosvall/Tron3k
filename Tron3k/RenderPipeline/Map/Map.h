#ifndef MAP_H
#define MAP_H

#include "../Mesh.h"



class Map
{

private:


public:
	void init(std::vector<Mesh> &meshes);

	void loadMap(std::string mapName);

};

#endif