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

	bool checkAABBCollision(glm::vec3 pos, CollideMesh mesh);
	bool checkAABBCollision(Geometry* obj1, Geometry* obj2);

	bool checkOBBCollision(glm::vec3 pos, CollideMesh mesh);
	bool checkOBBCollision(Geometry* obj1, Geometry* obj2);

	glm::vec3 minExtremes = glm::vec3(0, 0, 0);
	glm::vec3 maxExtremes = glm::vec3(0, 0, 0);
	glm::vec3 size = glm::vec3(0, 0, 0);
public:
	Physics();
	~Physics();

	virtual bool init();
	virtual bool release();

	
	virtual bool checkPlayerVPlayerCollision(glm::vec3 playerPos1, glm::vec3 playerPos2);
	virtual bool checkPlayerVBulletCollision(glm::vec3 playerPos, glm::vec3 bulletPos);
	virtual bool checkPlayerVWorldCollision(glm::vec3 playerPos);

	virtual void addGravity(glm::vec3 &velocity, float dt);

	//TEMPORARY
	virtual void getBoatExtremes(glm::vec3 minVals, glm::vec3 maxVals);
	virtual void receiveWorldBoxes(std::vector<std::vector<float>> wBoxes);
};

extern "C" PHYSICS_API Physics* CreatePhysics();

#endif