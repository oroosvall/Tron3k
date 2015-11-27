#ifndef GAME_H
#define GAME_H

#include "GameDataIndex.h"
#include "Role/Weapon/BulletTypes/AllBullets.h"
#include "Player.h"
#include "../../../Physics/Physics.h"
#include <vector>
#include <fstream>

enum Gamestate
{
	START,
	MENU,
	ROAM,
	CLIENT,
	SERVER
};

class Game
{
private:

	std::vector<Bullet*> bullets[BULLET_TYPE::NROFBULLETS];

	std::vector<int> teamSpectators; //Team vectors hold connection IDs
	std::vector<int> teamOne;
	std::vector<int> teamTwo;

	int maxTeamSize = 5;
	int maxSpec = 5;
	void removeConIDfromTeams(int conID);

	Role* templateRole;
	string loadedRoles[NROFROLES][NROFREAD];

	int gameState;

	int max_con;
	Player** playerList;
	Physics* physics;

	void loadRoles();
	void initPhysics();

	void registerWeapon(Player* p);
	void registerSwitch(Player* p);

	void addBulletToList(int conID, int bulletId, BULLET_TYPE bt, glm::vec3 pos, glm::vec3 dir);
	Bullet* getBulletForRemoval(int PID, int BID, BULLET_TYPE bt, int &posInBulletArray);

	WEAPON_TYPE weaponShotWith;
	int bulletShot = -1;
	bool shotsFired = false;

	WEAPON_TYPE weaponSwitchedTo;
	bool wpnSwitched = false;

	BulletHitInfo hit;
	bool playerHit = false;

	bool localPlayerWantsRespawn = false;
	bool localPlayerRespawnWaiting = false;
public:

	Game();
	void release();
	void init(int max_connections, int state);

	Player* getPlayer(int conID);
	std::vector<Bullet*> getBullets(BULLET_TYPE type);
	void createPlayer(Player* p, int conID, bool isLocal = false);
	void removePlayer(int conID);

	void update(float dt);

	void sendWorldBoxes(std::vector<std::vector<float>> wBoxes);
	void sendPlayerBox(std::vector<float> pBox);

	//Collision checks
	void checkPvPCollision();
	void checkPlayerVBulletCollision();
	void checkPlayerVWorldCollision();
	void checkBulletVWorldCollision();

	bool playerWantsToRespawn();
	void allowPlayerRespawn(int p_conID, glm::vec3 respawnPosition);
	void denyPlayerRespawn(char tryAgain);

	void addPlayerToTeam(int p_conID, int team);
	int getPlayersOnTeam(int team);
	int getMaxTeamSize(bool spec = false) { if (spec) return maxSpec; return maxTeamSize; };

	bool fireEventReady() { return shotsFired; };
	void getLatestWeaponFired(int localPlayer, WEAPON_TYPE &wt, int &bulletId);
	void handleWeaponFire(int conID, int bulletId, WEAPON_TYPE weapontype, glm::vec3 pos, glm::vec3 dir);

	bool weaponSwitchReady() { return wpnSwitched; };
	WEAPON_TYPE getWpnSwitch() { wpnSwitched = false; return weaponSwitchedTo; };
	void handleWeaponSwitch(int conID, WEAPON_TYPE ws);

	bool hitEventReady() { return playerHit; };
	BulletHitInfo getHitInfo() { playerHit = false; return hit; };
	int handleBulletHitEvent(BulletHitInfo hi, int newHPtotal = -1); //Returns the new HP total of the player, takes the new HP total instead if info is coming from server


	bool freecam; // freecam is active also when in spectate but specctate overides
	int spectateID; // -1 = none, else use conID

	float lastDT = 0;
};

#endif