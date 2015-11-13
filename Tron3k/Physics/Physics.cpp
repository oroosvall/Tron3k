#include "Physics.h"

Physics::Physics()
{

}

Physics::~Physics()
{

}

bool Physics::init()
{
	return 1;
}

bool Physics::release()
{
	return 1;
}

bool Physics::checkCollision(Geometry* geo1, Geometry* geo2)
{
	return false;
}

Physics* CreatePhysics()
{
	return new Physics();
}