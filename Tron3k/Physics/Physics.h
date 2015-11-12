#ifndef PHYSICS_H
#define PHYSICS_H

#ifdef PHYSICS_EXPORTS
#define PHYSICS_API __declspec( dllexport)
#else
#define PHYSICS_API __declspec( dllimport  )
#endif

#include "Collision\Geometry.h"
#include "Collision\CollideMesh.h"

class Physics
{
private:
	//General physics components will go here, and things will be added as we go

	bool checkCollision(CollideMesh* obj1, CollideMesh* obj2);//Iterative function for each layer of CollideMesh
public:
	Physics();
	~Physics();

	bool checkCollision(Geometry* obj1, Geometry* obj2);
};

extern "C" PHYSICS_API Physics* CreatePhysics();

#endif