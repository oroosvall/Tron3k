#include "SoundPlayer.h"
int SoundPlayer::nrOfSoundsPlaying = 0;
float SoundPlayer::soundVolume = 50.0f;
float SoundPlayer::musicVolume = 50.0f;
bool SoundPlayer::soundEnabler = false;
bool SoundPlayer::initialized = false;
#include <iostream>

//#include <vld.h>

SoundPlayer* SoundPlayer::singleton = nullptr;

void SoundPlayer::release()
{
	if (singleton)
	{
		delete singleton;
	}
}

void SoundPlayer::init(SoundPlayer* sound, int activateSound)
{
	singleton = sound;
	soundEnabler = activateSound;

	if (soundEnabler)
	{
		singleton->soundList[SOUNDS::soundEffectPoopRifleShot].loadFromFile("GameFiles/Sound/soundEffectPoopRifleShot.ogg");
		singleton->soundList[SOUNDS::soundEffectBulletPlayerHit].loadFromFile("GameFiles/Sound/soundEffectBulletPlayerHit.ogg");
		singleton->soundList[SOUNDS::soundEffectPusleRifleShot].loadFromFile("GameFiles/Sound/soundEffectPusleRifleShot.ogg");
		singleton->soundList[SOUNDS::firstBlood].loadFromFile("GameFiles/Sound/voiceFirstBlood.ogg");
		singleton->musicList[MUSIC::mainMenu] = "GameFiles/Sound/musicMainMenu.ogg";

		initialized = true;
	}
}

SoundPlayer* SoundPlayer::getSound()
{
	return singleton;
}

SoundPlayer::SoundPlayer()
{

}

SoundPlayer::~SoundPlayer()
{
	
}

void SoundPlayer::enableSounds()
{
	if (!soundEnabler)
	{
		for (int i = 0; i < nrOfSoundsPlaying; i++)
		{
			sounds[i].setVolume(soundVolume);
		}
		soundEnabler = true;
		playMusic(mainMenu);
		musicPlayer.setVolume(musicVolume);
	}
	else
	{
		for (int i = 0; i < nrOfSoundsPlaying; i++)
		{
			sounds[i].stop();
		}
		musicPlayer.stop();
		soundEnabler = false;
	}
}
bool SoundPlayer::getSoundEnabler()
{
	return soundEnabler;
}

bool SoundPlayer::getInitialized()
{
	return initialized;
}

void SoundPlayer::setVolumeMusic(float volume)
{
	musicPlayer.setVolume(volume);
}

void SoundPlayer::setVolumeSound(float volume)
{
	musicPlayer.setVolume(volume);
}

int SoundPlayer::playUserGeneratedSound(int sound)
{
	if (soundEnabler && initialized == 1)
	{
		sounds[nrOfSoundsPlaying].setBuffer(soundList[sound]);
		sounds[nrOfSoundsPlaying].play();
		nrOfSoundsPlaying++;
		nrOfSoundsPlaying %= MAXSOUNDS;
	}

	return 0;
}

int SoundPlayer::playExternalSound(int sound, float x, float y, float z)
{
	if (soundEnabler && initialized == 1)
	{

		sf::Listener::setDirection(playerDir.x, playerDir.y, playerDir.z);			//Set the direction of the player
		if (sf::Listener::getPosition().x > x - 0.1 && sf::Listener::getPosition().x < x + 0.1)
		{
			if (sf::Listener::getPosition().z > z - 0.1 && sf::Listener::getPosition().z < z + 0.1)
			{
				sounds[nrOfSoundsPlaying].setRelativeToListener(true);
				sounds[nrOfSoundsPlaying].setPosition(0, 0, 0);

			}
		}

		else
		{
			sounds[nrOfSoundsPlaying].setRelativeToListener(false);
			sounds[nrOfSoundsPlaying].setPosition(x, y, z);			//Set the sound's position in the world. Could be passed in through a parameter.
		}

		//std::cout << "x: " << sf::Listener::getPosition().x << " y: " << sf::Listener::getPosition().y << " z: " << sf::Listener::getPosition().z << endl;
		//std::cout << "x: " << x << " y: " << y << " z: " << z << endl;
		//sounds[nrOfSoundsPlaying].isRelativeToListener();
		sounds[nrOfSoundsPlaying].setMinDistance(10.0f);		//Set the sound's distance it travels before it starts to attenuate. Could be passed in through a parameter.

		sounds[nrOfSoundsPlaying].setBuffer(soundList[sound]);
		sounds[nrOfSoundsPlaying].setVolume(soundVolume);
		sounds[nrOfSoundsPlaying].play();
		nrOfSoundsPlaying++;
		nrOfSoundsPlaying %= MAXSOUNDS;
	}

	return 0;
}

void SoundPlayer::setLocalPlayerDir(glm::vec3 playerDir)
{
	this->playerDir = playerDir;
}

void SoundPlayer::setLocalPlayerPos(glm::vec3 playerPos)
{
	//cout << playerPos.x << "    " << playerPos.z << endl;
	sf::Listener::setPosition(playerPos.x, playerPos.y, playerPos.z);
}

int SoundPlayer::playMusic(int music)
{
	if (soundEnabler == 1 && initialized == 1)
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

SoundPlayer* CreateSound()
{
	return new SoundPlayer();
}

SoundPlayer* GetSound()
{
	return SoundPlayer::getSound();
}

bool GetSoundActivated()
{
	return SoundPlayer::getSoundEnabler();
}

bool GetInitialized()
{
	return SoundPlayer::getInitialized();
}

void InitSound(SoundPlayer* sound, int activateSound)
{
	SoundPlayer::init(sound, activateSound);
}

void ReleaseSound()
{
	SoundPlayer::release();
}