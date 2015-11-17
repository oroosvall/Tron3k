#ifndef NET_INDEX_H
#define NET_INDEX_H

enum NET_INDEX
{
	PING = int('P'),
	NEW_CONNECTION = int('N'),
	EVENT = int('E'),
	FRAME = int('F'),
	MESSAGE = int('M')
};

enum NET_NEW
{

};

enum NET_EVENT
{
	EMPTY = int('E'),
	SERVER_STATUS = int('S'),
	PLAYER_JOINED = int('J'),
	PLAYER_LEFT = int('L'),
	DAMAGE = int('D')
};

enum NET_FRAME
{
	POS = int('P'),
	JUMP = int('J'),
	FIRE = int('F'),
	NAME_CHANGE = int('N'),
	TEAM_CHANGE = int('T')
};

enum NET_MESSAGE
{
	ALL = int('A'),
	TEAM = int('T'),
	WHISPER = int('W')
};

#endif