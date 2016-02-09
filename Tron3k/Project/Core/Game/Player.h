#ifndef PLAYER_H
#define PLAYER_H

#include "GameObject.h"
#include "../../SoundPlayer/sound/SoundPlayer.h"
#include "../Input.h"
#include "../CameraInput.h"
#include "../sharedStructs.h" //spawnpoints transfere from renderpipe to game/player
#include "../AnimationState.h"

#include "../../../Physics/Physics.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Role/role.h"
#include "Role/Weapon/Weapon.h"
#include "Role/Weapon/bullet.h"
#include "Role/PlayerEffects/Effect.h"
#include "Role/PlayerEffects/Modifier.h"
#include "Role/Special/Special.h"
#include <iostream>

#include <string>

#define interpolationTick 0.050f
#define respawnTime 5.0f;

enum PLAYERMSG { NONE, SHOOT, USEITEM, SPECIALUSE, MOBILITYUSE, WPNSWITCH, DEATH, PLAYERRESPAWN};

class Player : public GameObject
{
private:
	//Physics* physics;
	Role role;
	bool lockControls = false;
	bool noclip = false;

	bool ready = false; //Ready to start a game mode

	bool justRespawned = false; // used for culling
	bool diedThisFrame = false; // used for K/D and messages

	bool footstepsLoop = false;
	float footstepsCountdown = 0;
	std::string name;
	glm::vec3 pos; //Current actual position
	glm::vec3 dir; //Current viewing direction
	glm::vec3 vel; //Our velocity i.e. in which direction we're moving
	glm::vec3 airVelocity; //Saves the velocity we hold in the air

	float maxspeed = 5.0f;

	glm::vec3 collisionVel; //How we ought to be moving based on our collisions
	void movePlayer(float dt, glm::vec3 oldDir, bool freecam, bool specingThis);
	void movePlayerCollided(float dt, glm::vec3 oldDir, bool freecam, bool specingThis);
	bool grounded = false;
	bool collided = false;

	glm::vec4 collisionNormals[20];
	int collisionNormalSize = 0;
	void clearCollisionNormals() { collisionNormalSize = 0; };

	bool isDead = false;
	float respawnTimer = 0.0f;

	glm::vec3 oldPos;
	glm::vec3 oldDir;
	glm::vec3 goalpos; //Used by non-local players to move smoothly
	glm::vec3 goaldir; //Used by non-locals to turn smoothly
	float goalTimer;

	AnimationState anim_first_current;
	AnimationState anim_first_framePeak; //send the highest ranked anim used since last frame
	AnimationState anim_third_current;
	AnimationState anim_third_framePeak;
	bool animGrounded = false;
	bool animGroundedLast = false;
	float animAirTimer = 0.0f;
	int animRole = 0;
	bool animPrimary = true;
	void movementAnimationChecks(float dt);
	float animSwapTime_OUT = 0;
	bool animSwapActive = false;
	bool lastanimSwapActive = false;
	
	void peakAnimsUpdate();
	bool animLastDead = false;

	bool isLocalPlayer;

	int currentTeam;

	Input* i;
	CameraInput* cam;

	vector<Modifier*> myModifiers;
	void modifiersGetData(float dt); //Gets relevant data (if any) from the player before update occurs
	void modifiersSetData(float dt); //Sets player's data as necessary after player's update occurs
	void cleanseModifiers(bool stickies = false); //Stickies are class passives AND code for mobility abilities
	bool removeSpecificModifier(MODIFIER_TYPE mt);

	void rotatePlayer(vec3 olddir, vec3 newdir);
	void reloadCurrentWeapon();

	int kills;
	int deaths;
	int consecutiveFrags = 0;
	int consecutiveDeaths = 0;
public:
	Player();
	~Player();
	void init(std::string name, glm::vec3 pos, bool isLocal = false);

	bool getLockedControls(){ return lockControls; };
	void setLockedControls(bool lockControls) { this->lockControls = lockControls; };

	bool isReady() { return ready; };
	void setReady() { ready = true; };
	void unReady() { ready = false; };

	void footstepsLoopReset(float dt);

	bool getFootsteps();
	void setFootstepsCountdown();
	void setFootstepsLoop(bool);
	int GetConsecutiveFrags();
	void IncreaseFrags();
	void IncreaseDeaths();
	void ZeroFrags();
	void ZeroDeaths();
	bool killingSpreeDone = false;
	bool impressiveDone = false;
	bool roleChangeWritten = false;

	PLAYERMSG update(float dt, bool freecam, bool spectatingThisPlayer, bool spectating);
	void movementUpdates(float dt, bool freecam, bool spectatingThisPlayer, bool spectating);

	void setName(std::string newName);
	void setGoalPos(glm::vec3 newPos);
	void setGoalDir(glm::vec3 newDir);

	void IdiedThisFrame() { diedThisFrame = true; };
	bool didIDieThisFrame() { return diedThisFrame; };

	void setCollisionInfo(std::vector<glm::vec4> collNormals);
	glm::vec4* getCollisionNormalsForFrame(int &size) { size = collisionNormalSize; return collisionNormals; };
	void setExplodingInfo(std::vector<glm::vec4> expDirs);
	void applyGravity (float vel);

	AnimationState getAnimState_f_c() { return anim_first_current; };
	AnimationState getAnimState_f_p() { return anim_first_framePeak; };
	AnimationState getAnimState_t_c() { return anim_third_current; };
	AnimationState getAnimState_t_p() { return anim_third_framePeak; };
	void setAnimState_f_c(AnimationState set) { anim_first_current = set; };
	void setAnimState_f_p(AnimationState set) { anim_first_framePeak = set; };
	void setAnimState_t_c(AnimationState set) { anim_third_current = set; };
	void setAnimState_t_p(AnimationState set) { anim_third_framePeak = set; };

	std::string getName() { return name; };
	glm::vec3 getPos() { if (length(pos) > 0.0f) return pos; return vec3(0.0f); };
	void setPos(glm::vec3 pos) 
	{ 
		this->pos = pos;
	}
	void setDir(glm::vec3 wantedDir) { dir = wantedDir; };
	glm::vec3 getDir() { return dir; };
	int getHP() { return role.getHealth(); };
	void setHP(int HPfromServer) { role.setHealth(HPfromServer); if (role.getHealth() > 0) isDead = false; }; //Used by client

	int getAmmo();
	int getMaxAmmo();

	bool isLocal() { return isLocalPlayer; };
	bool isAlive() { return !isDead; };

	Weapon* getPlayerCurrentWeapon();
	void switchWpn(WEAPON_TYPE ws, int swapLoc);
	void shoot();

	Special* getPlayerSpecialAbility() { return role.getSpecialAbility(); };

	void setTeam(int teamid) { currentTeam = teamid; };
	int getTeam() { return currentTeam; };

	void setGrounded(bool grounded) { this->grounded = grounded; };
	bool getGrounded() { return grounded; };

	void addCollisionNormal(glm::vec4 cn) {if (collisionNormalSize < 20){ collisionNormals[collisionNormalSize] = cn; collisionNormalSize++; } };

	void setVelocity(glm::vec3 velocity) { vel = velocity; if (!grounded) airVelocity = velocity; };
	glm::vec3 getVelocity() { return vel; };
	glm::vec3 getAirVelocity() { return airVelocity; };

	void hitByBullet(Bullet* b, BULLET_TYPE bt, int newHPtotal = -1);
	void hitByEffect(Effect* e, int newHPtotal = -1);

	void addModifier(MODIFIER_TYPE mt);

	void setRole(Role role);
	void chooseRole(int r);

	void respawn(glm::vec3 respawnPos, glm::vec3 dir, int roomID);
	bool getJustRespawned() { return justRespawned; };

	void healing(int amount);

	Role* getRole() { return &role; };

	glm::mat4 getFPSmat();

	void movmentSpecialAnimUse(int react = 0);
	bool getAnimPrimary();
	bool searchModifier(MODIFIER_TYPE search);
	Modifier* searchModifierGet(MODIFIER_TYPE search);

	int roomID;

	void clearKD() { kills = 0; deaths = 0; this->ZeroFrags(); };
	int getKills() { return kills; };
	int getDeaths() { return deaths; };
	void addKill() { kills++; };
	void addDeath() { deaths++;	};

	bool nameChanged;

	int GetConsecutiveDeaths() { return this->consecutiveDeaths; };
};

#endif