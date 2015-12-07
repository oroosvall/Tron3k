#ifndef GAME_DATA_INDEX_H
#define GAME_DATA_INDEX_H

enum WEAPON_TYPE
{
	PULSE_RIFLE,
	ENERGY_BOOST,
	DISC_GUN,
	MELEE,
	BATTERYFIELD,
	LINK_GUN,
	ENERGY_SHIELD,
	PLASMA_AUTORIFLE,
	GRENADE_LAUNCHER,
	SHOTGUN,
	NROFWEAPONS
};

enum SPECIAL_TYPE
{
	LIGHTWALL,
	MULTIJUMP,
	NROFSPECIALS
};

enum BULLET_TYPE
{
	PULSE_SHOT,
	DISC_SHOT,
	BATTERY_SPEED_SHOT,
	BATTERY_SLOW_SHOT,
	LINK_SHOT,
	PLASMA_SHOT,
	GRENADE_SHOT,
	SHOTGUN_PELLET,
	CLUSTER_GRENADE,
	CLUSTERLING,
	CLEANSE_BOMB,
	HACKING_DART,
	VACUUM_GRENADE,
	NROFBULLETS
};

enum CONSUMABLE_TYPE
{
	CLUSTERGRENADE,
	NROFCONSUMABLES
};

enum MODIFIER_TYPE
{
	LIGHTWALLCONTROLLOCK,
	NROFMODIFIERS
};

enum EFFECT_TYPE
{
	LIGHT_WALL,
	NROFEFFECTS
};

#endif