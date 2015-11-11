#include "SoundPlayer.h"


SoundPlayer::SoundPlayer()
{
	soundPlayer.setAttenuation(10.0f); //NEW! Sets the global attenuation of all the sounds.
	soundEnabler = true;
}

SoundPlayer::~SoundPlayer()
{

}

void SoundPlayer::enableSounds(bool enabler)
{
	soundEnabler = enabler;
}

void SoundPlayer::setVolumeMusic(int volume)
{
	musicPlayer.setVolume(volume);
}

void SoundPlayer::setVolumeSound(int volume)
{
	musicPlayer.setVolume(volume);
}

int SoundPlayer::playUserGeneratedSound(int sound)
{
	if (soundEnabler)
	{

		if (!soundBuffer.loadFromFile("tester.ogg"))
		{
			return -1;
		}

		soundPlayer.setBuffer(soundBuffer);
		soundPlayer.play();
	}

	return 0;
}

int SoundPlayer::playExternalSound(int sound, glm::vec3 soundOrigin)
{
	//sf::Listener::setPosition(x, y, z);			Set the position of the player
	//sf::Listener::setDirection(x, y, z);			Set the direction of the player

	if (soundEnabler)
	{

		if (!soundBuffer.loadFromFile("tester.ogg"))
		{
			return -1;
		}

		//soundPlayer.setMinDistance(parameter);		Set the sound's distance it travels before it starts to attenuate. Could be passed in through a parameter.
		//soundPlayer.setPosition(soundOrigin):			Set the sound's position in the world. Could be passed in through a parameter.
		soundPlayer.setBuffer(soundBuffer);
		soundPlayer.play();
	}

	return 0;
}

int SoundPlayer::playMusic(int music)
{
	if (soundEnabler)
	{

		if (!musicPlayer.openFromFile("musicTester1.ogg"))
		{
			return -1;
		}

		musicPlayer.play();
	}
	return 0;
}