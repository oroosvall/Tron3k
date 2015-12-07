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
	EMPTY =				int('E'),
	SERVER_STATUS =		int('S'),
	PLAYER_JOINED =		int('J'),
	PLAYER_LEFT =		int('L'),
	PLAYER_HIT =		int('H'),
	RESPAWN_DENIED =	int('R') //Used by command_respawn to deny respawn auth to client
};

enum NET_FRAME
{
	POS =			int('P'),
	FIRE =			int('F'),
	SPECIAL =		int('S'),
	WPN_SWITCH =	int('W'),
	NAME_CHANGE =	int('N'),
	CONSUMABLE =	int('C')
};

enum NET_COMMAND
{
	TEAM_CHANGE =	int('T'), //request teamchange
	RESPAWN =		int('R') //Request respawn
};

enum NET_MESSAGE
{
	ALL =		int('A'),
	TEAM =		int('T'),
	WHISPER =	int('W')
};

#endif