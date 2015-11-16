#include "SoundPlayer.h"


SoundPlayer::SoundPlayer()
{
	nrOfSoundsPlaying = 0;
	soundEnabler = true;

	soundList[SOUNDS::gunshot].loadFromFile("soundEffectGunshots.ogg");
	soundList[SOUNDS::firstBlood].loadFromFile("voiceFirstBlood.ogg");

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
		sounds[nrOfSoundsPlaying].setBuffer(soundList[sound]);
		sounds[nrOfSoundsPlaying].play();
		nrOfSoundsPlaying++;
	}

	return 0;
}

int SoundPlayer::playExternalSound(int sound, sf::Vector3f soundOrigin)
{
	//sf::Listener::setPosition(playerPosX, playerPosY, playerPosZ);			//Set the position of the player
	sf::Listener::setDirection(0.0f, 0.0f, -10.0f);			//Set the direction of the player

	if (soundEnabler)
	{
		sounds[nrOfSoundsPlaying].setMinDistance(10.0f);		//Set the sound's distance it travels before it starts to attenuate. Could be passed in through a parameter.
		sounds[nrOfSoundsPlaying].setPosition(soundOrigin);			//Set the sound's position in the world. Could be passed in through a parameter.
		sounds[nrOfSoundsPlaying].setBuffer(soundList[sound]);
		sounds[nrOfSoundsPlaying].play();
		nrOfSoundsPlaying++;
	}

	return 0;
}

int SoundPlayer::playMusic(int music)
{
	if (soundEnabler)
	{

		if (!musicPlayer.openFromFile(musicList[music]))
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

void SoundPlayer::update()
{
	for (int i = 0; i < nrOfSoundsPlaying; i++)
	{
		if(sounds[i].getStatus() == sf::Sound::Stopped)
		{
			sounds[nrOfSoundsPlaying] = sounds[i];
			nrOfSoundsPlaying--;
		}
	}
}