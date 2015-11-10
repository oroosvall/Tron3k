#include "Sound.h"

Sound::Sound()
{

}

Sound::~Sound()
{

}

int Sound::playSound(int sound)
{
	if (!soundBuffer.loadFromFile("tester.ogg"))
	{
		return -1;
	}

	soundPlayer.setBuffer(soundBuffer);

	return 0;
}

int Sound::playMusic(int music)
{
	if (!musicPlayer.openFromFile("musicTester.ogg"));
	{
		return -1;
	}

	musicPlayer.play();

	return 0;
}