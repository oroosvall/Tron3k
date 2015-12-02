#ifndef PHYSICS_H
#define PHYSICS_H

#ifdef PHYSICS_EXPORTS
#define PHYSICS_API __declspec( dllexport)
#else
#define PHYSICS_API __declspec( dllimport  )
#endif

#define GRAVITY 4.0f


#include "Collision\Geometry.h"
#include "Collision\CollideMesh.h"
#include<vector>


class Physics
{
private:
	//General physics components will go here, and things will be added as we go
	std::vector<CollideMesh> worldBoxes;
	CollideMesh playerBox;
	CollideMesh bulletBox;

	bool checkAABBCollision(CollideMesh mesh1, CollideMesh mesh2);
	bool checkAABBCollision(Geometry* obj1, Geometry* obj2);

	bool checkCylindervAABBCollision(CollideMesh mesh1, CollideMesh mesh2);

	bool checkOBBCollision(CollideMesh mesh1, CollideMesh mesh2);
	bool checkOBBCollision(Geometry* obj1, Geometry* obj2);

	glm::vec3 minExtremes = glm::vec3(0, 0, 0);
	glm::vec3 maxExtremes = glm::vec3(0, 0, 0);
	glm::vec3 size = glm::vec3(0, 0, 0);

	void initBulletBox();
public:
	Physics();
	~Physics();

	virtual bool init();
	virtual bool release();

	
	virtual bool checkPlayerVPlayerCollision(glm::vec3 playerPos1, glm::vec3 playerPos2);
	virtual bool checkPlayerVBulletCollision(glm::vec3 playerPos, glm::vec3 bulletPos);
	virtual bool checkPlayerVWorldCollision(glm::vec3 playerPos);
	virtual bool checkBulletVWorldCollision(glm::vec3 bulletPos);

	virtual void addGravity(glm::vec3 &velocity, float dt);

	virtual void receiveChunkBoxes(int chunkID, void* cBoxes);
	virtual void receiveWorldBoxes(std::vector<std::vector<float>> wBoxes);
	virtual void receivePlayerBox(std::vector<float> pBox);
};

extern "C" PHYSICS_API Physics* CreatePhysics();

#endif