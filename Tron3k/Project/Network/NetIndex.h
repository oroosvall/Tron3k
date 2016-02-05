#ifndef NET_INDEX_H
#define NET_INDEX_H

enum NET_INDEX
{
	PING =				int('P'),
	NEW_CONNECTION =	int('N'),
	EVENT =				int('E'),
	FRAME =				int('F'),
	COMMAND =			int('C'),
	MESSAGE =			int('M')
};

enum NET_EVENT
{
	EMPTY =					int('E'),
	SERVER_STATUS =			int('S'),
	GAMEMODE_DATA =			int('G'),
	PLAYERDATA =			int('P'),
	PLAYER_JOINED =			int('J'),
	PLAYER_LEFT =			int('L'),
	PLAYER_HIT =			int('H'),
	BULLET_WORLD_HIT =		int('W'),
	BULLET_EFFECT_HIT =		int('B'),
	EFFECT_PLAYER_HIT =		int('F'),
	BULLET_TIMEOUT =		int('T'),
	EFFECT_TIMEOUT =		int('I'),
	RESPAWN_DENIED =		int('R') //Used by command_respawn to deny respawn auth to client
};

enum NET_FRAME
{
	POS =			int('P'),
	ANIM =			int('A'),
	FIRE =			int('F'),
	SPECIAL =		int('S'),
	WPN_SWITCH =	int('W'),
	NAME_CHANGE =	int('N'),
	CONSUMABLE =	int('C')
};

enum NET_COMMAND
{
	TEAM_CHANGE =	int('T'), //request teamchange
	RESPAWN =		int('R'), //Request respawn
	ROLESWITCH =	int('C'),
	READY =			int('G')
};

enum NET_MESSAGE
{
	ALL =		int('A'),
	TEAM =		int('T'),
	WHISPER =	int('W')
};

#endif