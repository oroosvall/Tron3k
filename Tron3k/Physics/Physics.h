#ifndef PHYSICS_H
#define PHYSICS_H

#ifdef PHYSICS_EXPORTS
#define PHYSICS_API __declspec( dllexport)
#else
#define PHYSICS_API __declspec( dllimport  )
#endif

#define GRAVITY 18.0f
#define PI 3.14159265359
#define TAU 6.28318530718


#include "Collision\Meshes\CollideMesh.h"
#include "Collision\Meshes\BulletMesh.h"
#include "Collision\Meshes\CaptureMesh.h"
#include "Collision\Meshes\PlayerMesh.h"
#include "Collision\Meshes\RoomMesh.h"
#include "Collision\Meshes\WorldMesh.h"
#include "Effect\EffectMesh.h"
#include<vector>
#include<iterator>


//----COLLISION INFO----
/*
The first parameter for each collision check is ALWAYS The player or the bullet
The object we check against will ALWAYS be second parameter

It is ALWAYS the FIRST object COLLIDING with the SECOND
This means:
- We check the FIRST object towards a bunch of SECOND ones
- When we want the normal of the collision, it's the normal against the side of the SECOND OBJECT
	This is because we want to know in which direction to send the FIRST object
*/

class Physics
{
private:
	//General physics components will go here, and things will be added as we go
	//std::vector<std::vector<WorldMesh>> worldBoxes; //each CollideMesh is an ABB, and the vector is all CollideMeshes in the chunk
	std::vector<CaptureMesh> captureBoxes;
	
	std::vector<RoomMesh> roomBoxes;
	//each RoomBox is a room in the world
	//each also contains all the information for the objects inside it

	//std::vector<CollideMesh> captureBoxes;
	std::vector<EffectMesh*> effectBoxes;
	PlayerMesh playerBox;
	BulletMesh bulletBox;

	//--------AABB Collisions--------//
	bool checkAABBvAABBCollision(AABBSingle mesh1, AABBSingle mesh2);

	glm::vec3 checkAABBvAngledCylinderCollision(CollideMesh mesh1, CollideMesh mesh2);
	glm::vec3 checkAABBvCylinderCollision(CollideMesh mesh1, CollideMesh mesh2);
	
	glm::vec3 checkAABBvSphereCollision(CollideMesh mesh1, CollideMesh mesh2);
	//--------------//--------------//

	//--------OBB Collisions--------//
	glm::vec3 checkOBBvOBBCollision(CollideMesh mesh1, CollideMesh mesh2); //Probably not needed
	
	glm::vec4 checkOBBvCylinderCollision(Cylinder pMesh, OBB objMesh);
	glm::vec3 checkOBBvAngledCylinderCollision(CollideMesh mesh1, CollideMesh mesh2);
	
	
	//--------------//--------------//

	//-----Cylinder Collisions------//
	glm::vec3 checkCylindervCylinderCollision(CollideMesh mesh1, CollideMesh mesh2);
	glm::vec3 checkCylindervSphereCollision(CollideMesh mesh1, CollideMesh mesh2);
	glm::vec3 checkAngledCylindervSphereCollision(CollideMesh mesh1, CollideMesh mesh2);
	//--------------//--------------//

	//-------Sphere Collision-------//
	glm::vec4 checkSpherevSphereCollision(Sphere mesh1, Sphere mesh2);

	glm::vec4 checkSpherevSpheretdCollision(Sphere mesh1, Sphere mesh2);
	glm::vec4 checkSpherevOBBlwCollision(Sphere mesh1, OBB mesh2);
	//--------------//--------------//

	//--------Line Collision--------//
	vec3 checkLinevPlaneCollision(glm::vec3 l1, glm::vec3 l2, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 p4, glm::vec3 n);


	//------Normal Calculators------//
	std::vector<glm::vec3> getCollisionNormal(AABB* aabb1, AABB* aabb2);

	glm::vec4 getSpherevOBBlwNorms(glm::vec3 pos, float rad, OBB* obb);
	glm::vec4 getSpherevOBBNorms(glm::vec3 pos, float rad, OBB* obb, vec3 backDir = vec3(0), bool isBullet = false);
	//--------------//--------------//

	vec4 bulletNormal[4];

	void storeChunkBox(int chunkID, std::vector<AABB> cBox);
	void storeEffectBox(EffectMesh efml);

	void initBulletBox();
public:
	Physics();
	~Physics();

	virtual bool init();
	virtual bool release();

	virtual glm::vec3 normalize(glm::vec3 vec3);

	virtual bool removeEffect(int eid, int pid, unsigned int eType); //SKRIV EN FUNKTIONSFAN
	
	virtual glm::vec3 checkPlayerVPlayerCollision(glm::vec3 playerPos1, glm::vec3 playerPos2);
	virtual glm::vec3 checkPlayerVBulletCollision(glm::vec3 playerPos, glm::vec3 bulletPos, vec3 size, vec3 bulletDir, vec3 bulletVel, float dt, bool &collided, float bModifier = 1);
	virtual std::vector<glm::vec4> PlayerVWorldCollision(glm::vec3 playerPos, vec3 playerDir, vec3 playerVel, float dt);
	virtual glm::vec4 BulletVWorldCollision(glm::vec3 &bulletPos, vec3 bulletVel, vec3 bulletDir, float dt);
	virtual glm::vec4 checkPlayerVEffectCollision(glm::vec3 playerPos, unsigned int eType, int eid, bool &col);
	virtual glm::vec4 checkBulletVEffectCollision(glm::vec3 &bulletPos, vec3 bulletVel, vec3 bulletDir, unsigned int eType, int eid, float dt);
	virtual bool checkEffectVEffectCollision(unsigned int eType1, unsigned int eType2, int eid1, int eid2, int pid1, int pid2);
	virtual bool checkPlayerVCaptureCollision(vec3 playerPos, int capID);

	virtual float addGravity(float dt);

	virtual void receiveChunkBoxes(int chunkID, void* cBoxes);
	virtual void receiveCap(int nrCaps, void* capBoxes);
	virtual void receiveRoomBoxes(void* roomboxes);
	virtual void receivePlayerBox(std::vector<float> pBox, float rad);
	virtual void receivePlayerRad(float rad);
	virtual void receiveEffectBox(std::vector<float> eBox, unsigned int etype, int pID, int eID);

	virtual void cullingPointvsRoom(glm::vec3* pos, int* arr_interIDs, int& interCount, int maxsize);
};

extern "C" PHYSICS_API Physics* CreatePhysics();

#endif