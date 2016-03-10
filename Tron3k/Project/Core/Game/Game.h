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
#include "../Console.h"
#include "Gamemode.h"
#include <iostream>

#include "Gamemodes/KingOfTheHill.h"

#define Max_Decals 250

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
	glm::vec3 hitDir;
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
	glm::vec3 playerPos;
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
	int bulletTeam;
	BULLET_TYPE bt;
	int effectPID;
	int effectID;
	EFFECT_TYPE et;
	glm::vec3 hitPos;
	glm::vec3 hitDir;
	glm::vec4 collisionNormal;
};

struct BulletTimeOutInfo
{
	int bulletPID;
	int bulletBID;
	BULLET_TYPE bt;
	glm::vec3 pos;
};

struct EffectTimeOutInfo
{
	int effectPID;
	int effectID;
	EFFECT_TYPE et;
	glm::vec3 pos;
};

struct EffectHitEffectInfo
{
	float transp;
	int effectPID;
	int effectSID;
	EFFECT_TYPE et;
	int posInArr;
};

struct Decal_RenderInfo
{
	float inten;
	glm::vec3 pos;
	glm::vec3 color;
	glm::vec3 normal;
	float dummy;
	float dumm2;
};

struct Decal_GameInfo
{
	float lifeTime;
	float lifeLeft;

	bool update_true_if_dead(float dt, float& newInten)
	{
		lifeLeft -= dt;
		if (lifeLeft < 0)
			return true;

		newInten = lifeLeft / lifeTime;
		return false;
	}
};

struct HitPosAndDirParticle
{
	glm::vec3 pos;
	glm::vec3 dir;
	glm::vec3 color;
	BULLET_TYPE btype;
};

struct EffectParticle
{
	glm::vec3 pos;
	glm::vec3 color;
	EFFECT_TYPE etype;
};

class Game
{
private:
	Gamemode* gamemode;

	Console* console;

	std::vector <vector < SpawnpointG >> spawnpoints;

	std::vector<Bullet*> bullets[BULLET_TYPE::NROFBULLETS];
	std::vector<Effect*> effects[EFFECT_TYPE::NROFEFFECTS];

	Decal_GameInfo decals_gameInfo[Max_Decals];
	Decal_RenderInfo decals_renderInfo[Max_Decals];
	unsigned int decalCounter = 0;

	std::vector<int> teamSpectators; //Team vectors hold connection IDs
	std::vector<int> teamOne;
	std::vector<int> teamTwo;

	std::vector<string> suicideMessages;

	std::vector<string> fragMessages;

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

	void addBulletToList(int conID, int teamId, int bulletId, BULLET_TYPE bt, glm::vec3 pos, glm::vec3 dir);
	Bullet* getSpecificBullet(int PID, int BID, BULLET_TYPE bt, int &posInBulletArray);
	void removeBullet(BULLET_TYPE bt, int posInArray);
	void bounceBullet(BulletHitWorldInfo hwi, Bullet* theBullet);

	void addEffectToList(int conID, int teamId, int effectId, EFFECT_TYPE et, glm::vec3 pos, int dmg, float interestingVariable);
	Effect* getSpecificEffect(int PID, int SID, EFFECT_TYPE et, int &posInEffectArray);
	void removeEffect(EFFECT_TYPE et, int posInArray);
	void addEffectToPhysics(Effect* effect);

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
	std::vector<BulletTimeOutInfo> allBulletTimeOuts;
	std::vector<EffectTimeOutInfo> allEffectTimeOuts;

	std::vector<HitPosAndDirParticle> allBulletHitPlayerPos;
	std::vector<EffectParticle> allEffectParticleSpawn;

	std::vector<MovableParticle*> allMovableParticles;

	void checkPvPCollision();
	void checkPlayerVBulletCollision(float dt);
	void checkPlayerVWorldCollision(float dt);
	void checkBulletVWorldCollision(float dt);
	void checkBulletVEffectCollision(float dt);
	void checkPlayerVEffectCollision();
	void checkEffectVEffectCollision();
	void checkPlayerVCapCollision();

	void sendEffectBox(Effect* ef);

	bool localPlayerWantsRespawn = false;
	bool localPlayerRespawnWaiting = false;
public:

	vec3 TEAMONECOLOR = vec3(1.0f, 0.5f, 0.0f);
	vec3 TEAMTWOCOLOR = vec3(0.0f, 0.9f, 0.0f);
	vec3 SPEEDBUBBLECOLOR = vec3(0.9f, 0.0f, 0.0f);
	vec3 SLOWBUBBLECOLOR = vec3(0.0f, 0.4f, 1.0f);

	Game();
	void release();
	void init(int max_connections, int state, Console* con);

	int nrOfPlayersReady();
	void nrOfPlayersReadyReset();

	int musicVolumeForMenu = 30;

	Gamemode* getGameMode() { return gamemode; };
	int GetGameState();
	int getMaxCon();
	Player** getPlayerList();
	Player* getPlayer(int conID);
	std::vector<Bullet*> getBullets(BULLET_TYPE type);
	std::vector<Effect*> getEffects(EFFECT_TYPE type);
	void createPlayer(Player* p, int conID, int hp, int role, bool isLocal = false);
	void removePlayer(int conID);
	int GetLocalPlayerId();

	void update(float dt);

	void clearSpectateID() { spectateID = -1; };
	std::vector< std::vector < SpawnpointG > >* getSpawnpoints() { return &spawnpoints; };

	void sendChunkBoxes(int chunkID, void* cBoxes);
	void sendCapBoxes(int nrCaps, void* capBoxes);
	void sendRoomBoxes(void* roomboxes);
	void sendWorldBoxes(std::vector<std::vector<float>> wBoxes);
	void sendPlayerBox(std::vector<float> pBox);
	void sendPlayerRadSize(float rad);
	void updateEffectBox(Effect* effect);

	//Collision checks
	Physics* getPhysics() { return physics; };

	bool playerWantsToRespawn();
	void allowPlayerRespawn(int p_conID, int respawnPosition);
	void denyPlayerRespawn(char tryAgain);

	void addPlayerToTeam(int p_conID, int team);
	int getPlayersOnTeam(int team);
	int getMaxTeamSize(bool spec = false) { if (spec) return maxSpec; return maxTeamSize; };
	vector<int>* getTeamConIds(int team);

	bool fireEventReady() { return shotsFired; };
	void getLatestWeaponFired(int localPlayer, WEAPON_TYPE &wt, int &bulletId);
	void handleWeaponFire(int conID, int teamId, int bulletId, WEAPON_TYPE weapontype, glm::vec3 pos, glm::vec3 dir);

	bool consumableReady() { return consumableUsed; };
	CONSUMABLE_TYPE getConsumableUsed(int localPlayer);
	void handleConsumableUse(int conID, int teamId, CONSUMABLE_TYPE ct, glm::vec3 pos, glm::vec3 dir);

	bool weaponSwitchReady() { return wpnSwitched; };
	WEAPON_TYPE getWpnSwitch(int &swaploc) { wpnSwitched = false; swaploc = wpnSwapLocation; return weaponSwitchedTo; };
	void handleWeaponSwitch(int conID, WEAPON_TYPE ws, int swapLoc);

	bool specialActivationReady() { return specialActivated; };
	bool mobilityActivationReady() { return mobilityActivated; };

	SPECIAL_TYPE getMobilityAbilityUsed(int localPlayer, int &sid);
	SPECIAL_TYPE getSpecialAbilityUsed(int localPlayer, int &sid); //sid is SpecialId, really only used for Lightwalls. I'm so, so sorry
	void handleSpecialAbilityUse(int conID, int teamId, int specialId, SPECIAL_TYPE st, glm::vec3 pos, glm::vec3 dir);

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
	void clearBulletOnEffectCollisions() { allBulletHitsOnEffects.clear(); };	
	void handleBulletHitEffectEvent(BulletHitEffectInfo hi);

	std::vector<EffectHitEffectInfo> getAllEffectOnEffectCollisions() { return allEffectHitsOnEffects; };
	void clearEffectOnEffectCollision() { allEffectHitsOnEffects.clear(); };
	void handleEffectHitEffectEvent(EffectHitEffectInfo hi);

	std::vector<BulletTimeOutInfo> getAllTimedOutBullets() { return allBulletTimeOuts; };
	void clearTimedOutBullets() { allBulletTimeOuts.clear(); };
	void handleBulletTimeOuts(BulletTimeOutInfo hi);

	std::vector<EffectTimeOutInfo> getAllTimedOutEffects() { return allEffectTimeOuts; };
	void clearTimedOutEffects() { allEffectTimeOuts.clear(); };
	void handleEffectTimeOuts(EffectTimeOutInfo hi);

	std::vector<HitPosAndDirParticle> getAllBulletHitPlayerPos() { return allBulletHitPlayerPos; };
	void clearAllBulletHitPlayerPos() { allBulletHitPlayerPos.clear(); };

	std::vector<EffectParticle> getAllEffectParticleSpawn() { return allEffectParticleSpawn; };
	void clearAllEffectParticleSpawn() { allEffectParticleSpawn.clear(); };

	std::vector<MovableParticle*>* getAllMovableParticle() { return &allMovableParticles; };

	unsigned int getNrOfDecals();
	Decal_GameInfo* getAllDecalGameInfo();
	Decal_RenderInfo* getAllDecalRenderInfo();
	void updateDecals(float dt);
	void setnewDecalInten(unsigned int id, float inten);
	void decalAdd(BulletHitWorldInfo info, float rad);

	int Game::findPlayerPosInTeam(int conID);

	void setPlayerWantsToRespawn(bool w) { localPlayerWantsRespawn = w; };
	bool checkIfPlayerCanRespawn(int conid, char &tryAgain);

	bool freecam; // freecam is active also when in spectate but specctate overides
	int spectateID; // -1 = none, else use conID
	void cullingPointvsRoom(glm::vec3* pos, int* arr_interIDs, int& interCount, int maxsize);

	float lastDT = 0;

	void clearAllPlayerKD();
	void resetAllPickups();

	void fixLocalPlayerCamera(float dt);
};

#endif