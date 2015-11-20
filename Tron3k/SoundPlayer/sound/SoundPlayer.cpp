#include "SoundPlayer.h"
int SoundPlayer::nrOfSoundsPlaying = 0;
float SoundPlayer::soundVolume = 100.0f;
float SoundPlayer::musicVolume = 50.0f;
bool SoundPlayer::soundEnabler = false;


SoundPlayer* SoundPlayer::singleton = nullptr;

void SoundPlayer::release()
{
	if (singleton)
	{
		delete singleton;
	}
}

void SoundPlayer::init(SoundPlayer* sound)
{
	singleton = sound;

	singleton->soundList[SOUNDS::gunshot].loadFromFile("GameFiles/Sound/soundEffectGunshots.ogg");
	singleton->soundList[SOUNDS::firstBlood].loadFromFile("GameFiles/Sound/voiceFirstBlood.ogg");
	singleton->musicList[MUSIC::mainMenu] = "GameFiles/Sound/musicMainMenu.ogg";
}

SoundPlayer* SoundPlayer::getSound()
{
	if (singleton == nullptr)
	{
		singleton = new SoundPlayer();
	}
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
			sounds[i].setVolume(0.0);
		}
		musicPlayer.setVolume(0.0);
		soundEnabler = true;
	}
	else
	{
		for (int i = 0; i < nrOfSoundsPlaying; i++)
		{
			sounds[i].setVolume(soundVolume);
		}
		musicPlayer.setVolume(musicVolume);
		soundEnabler = false;
	}
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
	if (nrOfSoundsPlaying < 29)
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

	if (nrOfSoundsPlaying < 29)
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

SoundPlayer* CreateSound()
{
	return new SoundPlayer();
}

SoundPlayer* GetSound()
{
	return SoundPlayer::getSound();
}

void InitSound(SoundPlayer* sound)
{
	SoundPlayer::init(sound);
}

void ReleaseSound()
{
	SoundPlayer::release();
}