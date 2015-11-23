#ifndef NET_INDEX_H
#define NET_INDEX_H

enum NET_INDEX
{
	PING = int('P'),
	NEW_CONNECTION = int('N'),
	EVENT = int('E'),
	FRAME = int('F'),
	COMMAND = int('C'),
	MESSAGE = int('M')
};

enum NET_EVENT
{
	EMPTY = int('E'),
	SERVER_STATUS = int('S'),
	PLAYER_JOINED = int('J'),
	PLAYER_LEFT = int('L'),
	HIT = int('H')
};

enum NET_FRAME
{
	POS = int('P'),
	JUMP = int('J'),
	FIRE = int('F'),
	WPN_SWITCH = int('W'),
	NAME_CHANGE = int('N'),
};

enum NET_COMMAND 
{
	TEAM_CHANGE = int('T') //request teamchange
};

enum NET_MESSAGE
{
	ALL = int('A'),
	TEAM = int('T'),
	WHISPER = int('W')
};

#endif