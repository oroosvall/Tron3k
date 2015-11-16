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
	soundBuffer.~SoundBuffer();
	sounds->~Sound();
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
	if (soundEnabler && nrOfSoundsPlaying < 29)
	{
		sounds[nrOfSoundsPlaying].setBuffer(soundList[sound]);
		sounds[nrOfSoundsPlaying].play();
		nrOfSoundsPlaying++;
	}
	else if (nrOfSoundsPlaying >= 30)
	{
		sounds[0].setMinDistance(10.0f);
		sounds[0].setPosition(0.0f, 0.0f, 0.0f);
		sounds[0].setBuffer(soundList[sound]);
		sounds[0].play();
	}

	return 0;
}

int SoundPlayer::playExternalSound(int sound, sf::Vector3f soundOrigin)
{
	//sf::Listener::setPosition(playerPosX, playerPosY, playerPosZ);			//Set the position of the player
	sf::Listener::setDirection(0.0f, 0.0f, -10.0f);			//Set the direction of the player

	if (soundEnabler && nrOfSoundsPlaying < 29)
	{
		sounds[nrOfSoundsPlaying].setMinDistance(10.0f);		//Set the sound's distance it travels before it starts to attenuate. Could be passed in through a parameter.
		sounds[nrOfSoundsPlaying].setPosition(soundOrigin);			//Set the sound's position in the world. Could be passed in through a parameter.
		sounds[nrOfSoundsPlaying].setBuffer(soundList[sound]);
		sounds[nrOfSoundsPlaying].play();
		nrOfSoundsPlaying++;
	}
	else if (nrOfSoundsPlaying >= 30)
	{
		sounds[0].setMinDistance(10.0f);		
		sounds[0].setPosition(soundOrigin);			
		sounds[0].setBuffer(soundList[sound]);
		sounds[0].play();
	}

	return 0;
}

int SoundPlayer::playMusic(int music)
{
	if (soundEnabler)
	{
		if (musicPlayer.getStatus() == sf::Sound::Playing)
		{
			musicPlayer.stop();
		}

		if (!musicPlayer.openFromFile(musicList[music]))
		{
			return -1;
		}

		musicPlayer.play();
		musicPlayer.setLoop(true);
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