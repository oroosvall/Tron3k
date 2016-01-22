#ifndef GAME_H
#define GAME_H

#include "GameDataIndex.h"
#include "Role/PlayerEffects/Effects/AllEffects.h"
#include "Role/Weapon/BulletTypes/AllBullets.h"
#include "Player.h"
#include "../../../Physics/Physics.h"
#include <vector>
#include <fstream>
#include "../sharedStructs.h"
#include <iostream>

enum Gamestate
{
	START,
	MENU,
	ROAM,
	CLIENT,
	SERVER
};

struct BulletHitPlayerInfo
{
	int playerHit;
	int bulletPID;
	int bulletBID;
	BULLET_TYPE bt;
	int newHPtotal;
};

struct EffectHitPlayerInfo
{
	int playerHit;
	int effectPID;
	int effectID;
	EFFECT_TYPE et;
	glm::vec3 hitPos;
	int newHPtotal;
};

struct BulletHitWorldInfo
{
	int bulletPID;
	int bulletBID;
	BULLET_TYPE bt;
	glm::vec3 hitPos;
	glm::vec3 hitDir; //For bouncing things
	glm::vec4 collisionNormal;
};

struct BulletHitEffectInfo
{
	int bulletPID;
	int bulletBID;
	BULLET_TYPE bt;
	glm::vec3 hitPos;
	glm::vec3 hitDir;
	glm::vec4 collisionNormal;
};

struct EffectHitEffectInfo
{
	int effectPID;
	int effectSID;
	EFFECT_TYPE et;
	int posInArr;
};

class Game
{
private:
	std::vector <vector < SpawnpointG >> spawnpoints;

	std::vector<Bullet*> bullets[BULLET_TYPE::NROFBULLETS];
	std::vector<Effect*> effects[EFFECT_TYPE::NROFEFFECTS];

	std::vector<int> teamSpectators; //Team vectors hold connection IDs
	std::vector<int> teamOne;
	std::vector<int> teamTwo;

	int maxTeamSize = 5;
	int maxSpec = 5;
	void removeConIDfromTeams(int conID);

	Role* templateRole;
	string loadedRoles[NROFROLES][NROFREADPROPERTIES];

	int gameState;

	int max_con;
	Player** playerList;
	Physics* physics;
	int localPlayerId = -1;

	void checkFootsteps(float dt);

	void loadRoles();
	void initPhysics();

	void addBulletToList(int conID, int bulletId, BULLET_TYPE bt, glm::vec3 pos, glm::vec3 dir);
	Bullet* getSpecificBullet(int PID, int BID, BULLET_TYPE bt, int &posInBulletArray);
	void removeBullet(BULLET_TYPE bt, int posInArray);
	void bounceBullet(BulletHitWorldInfo hwi, Bullet* theBullet);

	void addEffectToList(int conID, int effectId, EFFECT_TYPE et, glm::vec3 pos);
	Effect* getSpecificEffect(int PID, int SID, EFFECT_TYPE et, int &posInEffectArray);
	void removeEffect(EFFECT_TYPE et, int posInArray);

	void playerUpdate(int conid, float dt);
	void playerApplyForces(int conid, float dt);

	void registerWeapon(Player* p);
	void registerSwitch(Player* p);
	void registerSpecial(Player* p);
	void registerMobility(Player* p);
	void registerConsumable(Player* p);

	WEAPON_TYPE weaponShotWith;
	int bulletShot = -1;
	bool shotsFired = false;

	WEAPON_TYPE weaponSwitchedTo;
	int wpnSwapLocation = -1;
	bool wpnSwitched = false;

	SPECIAL_TYPE specialUsed;
	SPECIAL_TYPE mobilityUsed;
	bool specialActivated = false;
	bool mobilityActivated = false;
	int specialId = -1;

	CONSUMABLE_TYPE itemUsed;
	bool consumableUsed = false;

	std::vector<BulletHitPlayerInfo> allBulletHitsOnPlayers;
	std::vector<EffectHitPlayerInfo> allEffectHitsOnPlayers;
	std::vector<BulletHitWorldInfo> allBulletHitsOnWorld;
	std::vector<BulletHitEffectInfo> allBulletHitsOnEffects;
	std::vector<EffectHitEffectInfo> allEffectHitsOnEffects;

	void checkPvPCollision();
	void checkPlayerVBulletCollision();
	void checkPlayerVWorldCollision(float dt);
	void checkBulletVWorldCollision();
	void checkBulletVEffectCollision();
	void checkPlayerVEffectCollision();

	void sendEffectBox(Effect* ef);

	bool localPlayerWantsRespawn = false;
	bool localPlayerRespawnWaiting = false;
public:


	Game();
	void release();
	void init(int max_connections, int state);

	int getMaxCon();
	Player** getPlayerList();
	Player* getPlayer(int conID);
	std::vector<Bullet*> getBullets(BULLET_TYPE type);
	std::vector<Effect*> getEffects(EFFECT_TYPE type);
	void createPlayer(Player* p, int conID, bool isLocal = false);
	void removePlayer(int conID);

	void update(float dt);

	void clearSpectateID() { spectateID = -1; };
	std::vector< std::vector < SpawnpointG > >* getSpawnpoints() { return &spawnpoints; };

	void sendChunkBoxes(int chunkID, void* cBoxes);
	void sendWorldBoxes(std::vector<std::vector<float>> wBoxes);
	void sendPlayerBox(std::vector<float> pBox);

	//Collision checks
	Physics* getPhysics() { return physics; };

	bool playerWantsToRespawn();
	void allowPlayerRespawn(int p_conID, int respawnPosition);
	void denyPlayerRespawn(char tryAgain);

	void addPlayerToTeam(int p_conID, int team, int spawnPosition);
	int getPlayersOnTeam(int team);
	int getMaxTeamSize(bool spec = false) { if (spec) return maxSpec; return maxTeamSize; };

	bool fireEventReady() { return shotsFired; };
	void getLatestWeaponFired(int localPlayer, WEAPON_TYPE &wt, int &bulletId);
	void handleWeaponFire(int conID, int bulletId, WEAPON_TYPE weapontype, glm::vec3 pos, glm::vec3 dir);

	bool consumableReady() { return consumableUsed; };
	CONSUMABLE_TYPE getConsumableUsed(int localPlayer);
	void handleConsumableUse(int conID, CONSUMABLE_TYPE ct, glm::vec3 pos, glm::vec3 dir);

	bool weaponSwitchReady() { return wpnSwitched; };
	WEAPON_TYPE getWpnSwitch(int &swaploc) { wpnSwitched = false; swaploc = wpnSwapLocation; return weaponSwitchedTo; };
	void handleWeaponSwitch(int conID, WEAPON_TYPE ws, int swapLoc);

	bool specialActivationReady() { return specialActivated; };
	bool mobilityActivationReady() { return mobilityActivated; };

	SPECIAL_TYPE getMobilityAbilityUsed(int localPlayer, int &sid);
	SPECIAL_TYPE getSpecialAbilityUsed(int localPlayer, int &sid); //sid is SpecialId, really only used for Lightwalls. I'm so, so sorry
	void handleSpecialAbilityUse(int conID, int specialId, SPECIAL_TYPE st, glm::vec3 pos, glm::vec3 dir);

	std::vector<BulletHitPlayerInfo> getAllHitPlayerInfo() { return allBulletHitsOnPlayers; };
	void clearBulletOnPlayerCollisions() { allBulletHitsOnPlayers.clear();};
	int handleBulletHitPlayerEvent(BulletHitPlayerInfo hi); //Returns the new HP total of the player, takes the new HP total instead if info is coming from server

	std::vector<EffectHitPlayerInfo> getAllEffectOnPlayerCollisions() { return allEffectHitsOnPlayers; };
	void clearEffectOnPlayerCollisions() { allEffectHitsOnPlayers.clear(); };
	int handleEffectHitPlayerEvent(EffectHitPlayerInfo hi); //Returns new HP total of player, if we are client then instead simply set HP coming from server

	std::vector<BulletHitWorldInfo> getAllBulletOnWorldCollisions() { return allBulletHitsOnWorld; };
	void clearBulletOnWorldCollisions() { allBulletHitsOnWorld.clear(); };
	void handleBulletHitWorldEvent(BulletHitWorldInfo hi);

	std::vector<BulletHitEffectInfo> getAllBulletOnEffectCollisions() { return allBulletHitsOnEffects; };
	void clearBulletOnEffectCollision() { allBulletHitsOnEffects.clear(); };	
	void handleBulletHitEffectEvent(BulletHitEffectInfo hi);

	std::vector<EffectHitEffectInfo> getAllEffectOnEffectCollisions() { return allEffectHitsOnEffects; };
	void clearEffectOnEffectCollision() { allEffectHitsOnEffects.clear(); };
	void handleEffectHitEffectEvent(EffectHitEffectInfo hi);


	int Game::findPlayerPosInTeam(int conID);

	bool freecam; // freecam is active also when in spectate but specctate overides
	int spectateID; // -1 = none, else use conID

	float lastDT = 0;
};

#endif