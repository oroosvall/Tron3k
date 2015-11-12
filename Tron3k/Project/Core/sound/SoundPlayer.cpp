#include "SoundPlayer.h"


SoundPlayer::SoundPlayer()
{
	soundPlayer.setAttenuation(10.0f); //NEW! Sets the global attenuation of all the sounds.
	soundEnabler = true;

	sounds[SOUNDS::gunshot].loadFromFile("soundEffectGunshot.ogg");
	sounds[SOUNDS::firstBlood].loadFromFile("voiceFirstBlood.ogg");

	musicList[MUSIC::mainMenu] = "musicMainMenu.ogg";
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
		soundPlayer.setPosition(0.0f, 0.0f, 0.0f);

		soundPlayer.setBuffer(sounds[sound]);
		soundPlayer.play();
	}

	return 0;
}

int SoundPlayer::playExternalSound(int sound, sf::Vector3f soundOrigin)
{
	//sf::Listener::setPosition(playerPosX, playerPosY, playerPosZ);			//Set the position of the player
	sf::Listener::setDirection(0.0f, 0.0f, -10.0f);			//Set the direction of the player

	if (soundEnabler)
	{

		if (!soundBuffer.loadFromFile("soundEffectGunshots.ogg"))
		{
			return -1;
		}

		soundPlayer.setMinDistance(10.0f);		//Set the sound's distance it travels before it starts to attenuate. Could be passed in through a parameter.
		soundPlayer.setPosition(soundOrigin);			//Set the sound's position in the world. Could be passed in through a parameter.
		soundPlayer.setBuffer(soundBuffer);
		soundPlayer.play();
	}

	return 0;
}

int SoundPlayer::playMusic(int music)
{
	if (soundEnabler)
	{

		if (!musicPlayer.openFromFile("musicMainMenu.ogg"))
		{
			return -1;
		}

		musicPlayer.play();
	}
	return 0;
}

void SoundPlayer::rotate(float deltaTime)
{
	sf::Listener::setDirection(cos(deltaTime), 0.0f, sin(deltaTime));
}