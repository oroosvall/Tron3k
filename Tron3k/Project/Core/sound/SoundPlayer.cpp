#include "SoundPlayer.h"

SoundPlayer::SoundPlayer()
{

}

SoundPlayer::~SoundPlayer()
{

}

int SoundPlayer::playSound(int sound)
{
	if (!soundBuffer.loadFromFile("tester.ogg"))
	{
		return -1;
	}

	soundPlayer.setBuffer(soundBuffer);
	soundPlayer.play();

	return 0;
}

int SoundPlayer::playMusic(int music)
{
	if (!musicPlayer.openFromFile("musicTester.ogg"))
	{
		return -1;
	}

	musicPlayer.play();

	return 0;
}