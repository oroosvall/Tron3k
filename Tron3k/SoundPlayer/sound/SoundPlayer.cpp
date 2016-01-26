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
		singleton->soundList[SOUNDS::soundEffectEnergyBoost].loadFromFile("GameFiles/Sound/soundEffectEnergyBoost.ogg");
		singleton->soundList[SOUNDS::soundEffectBulletPlayerHit].loadFromFile("GameFiles/Sound/soundEffectBulletPlayerHit.ogg");
		singleton->soundList[SOUNDS::soundEffectPusleRifleShot].loadFromFile("GameFiles/Sound/soundEffectPusleRifleShot.ogg");
		singleton->soundList[SOUNDS::firstBlood].loadFromFile("GameFiles/Sound/voiceFirstBlood.ogg");
		singleton->musicList[MUSIC::mainMenu] = "GameFiles/Sound/musicMainMenu.ogg";
		singleton->soundList[SOUNDS::soundEffectGrenadeLauncher].loadFromFile("GameFiles/Sound/soundEffectGrenadeLauncher.ogg");
		singleton->soundList[SOUNDS::soundEffectShotGun].loadFromFile("GameFiles/Sound/soundEffectShotGun.ogg");
		singleton->soundList[SOUNDS::soundEffectDiscGun].loadFromFile("GameFiles/Sound/soundEffectDiscGun.ogg");
		singleton->soundList[SOUNDS::soundEffectLinkGun].loadFromFile("GameFiles/Sound/soundEffectLinkGun.ogg");
		singleton->soundList[SOUNDS::soundEffectMelee].loadFromFile("GameFiles/Sound/soundEffectMelee.ogg");
		singleton->soundList[SOUNDS::soundFootSteps].loadFromFile("GameFiles/Sound/soundFootSteps.ogg");
		singleton->soundList[SOUNDS::soundStepsManipulator].loadFromFile("GameFiles/Sound/SoundStepsManipulator.ogg");
		singleton->soundList[SOUNDS::soundEffectTrapperReload].loadFromFile("GameFiles/Sound/soundEffectTrapperReload2.ogg");
		singleton->soundList[SOUNDS::soundEffectShield].loadFromFile("GameFiles/Sound/soundShield.ogg");
		singleton->soundList[SOUNDS::soundEffectDiscBounce].loadFromFile("GameFiles/Sound/soundEffectDiscBounce.ogg");
		singleton->soundList[SOUNDS::soundEffectBruteSteps].loadFromFile("GameFiles/Sound/soundEffectBruteSteps2.ogg");
		singleton->soundList[SOUNDS::soundEffectShankerSteps].loadFromFile("GameFiles/Sound/soundEffectShankerSteps.ogg");
		singleton->soundList[SOUNDS::soundEffectDestroyerSteps].loadFromFile("GameFiles/Sound/soundEffectDestroyerSteps.ogg");
		singleton->soundList[SOUNDS::soundEffectClusterGrenade].loadFromFile("GameFiles/Sound/soundEffectClusterGrenadeExplosion.ogg");
		singleton->soundList[SOUNDS::soundEffectDestroyerStop].loadFromFile("GameFiles/Sound/soundEffectDestroyerStop.ogg");
		singleton->soundList[SOUNDS::soundEffectDestroyerStart].loadFromFile("GameFiles/Sound/soundEffectDestroyerStart.ogg");
		singleton->soundList[SOUNDS::soundEffectVacuumGrenade].loadFromFile("GameFiles/Sound/soundEffectVacuumGrenade.ogg");
		singleton->soundList[SOUNDS::soundAids].loadFromFile("GameFiles/Sound/soundAids.ogg");
		singleton->musicList[MUSIC::cantinaSong] = "GameFiles/Sound/cantinaSong.ogg";
		singleton->soundList[SOUNDS::power].loadFromFile("GameFiles/Sound/Power.ogg");
		singleton->soundList[SOUNDS::soundEffectGrenadeBounce].loadFromFile("GameFiles/Sound/soundEffectGrenadeBounce.ogg");
		singleton->soundList[SOUNDS::soundEffectHackingDart].loadFromFile("GameFiles/Sound/soundEffectHackingDart.ogg");
		singleton->soundList[SOUNDS::soundEffectLightSpeed].loadFromFile("GameFiles/Sound/soundEffectLightSpeed2.ogg");
		singleton->soundList[SOUNDS::soundEffectTrapperJump].loadFromFile("GameFiles/Sound/soundEffectTrapperJump.ogg");
		singleton->soundList[SOUNDS::soundEffectTrapperLand].loadFromFile("GameFiles/Sound/soundEffectTrapperLand.ogg");
		singleton->soundList[SOUNDS::soundEffectTrapperMultiJump ].loadFromFile("GameFiles/Sound/soundEffectTrapperMultiJump.ogg");
		singleton->soundList[SOUNDS::soundEffectLightWall].loadFromFile("GameFiles/Sound/soundEffectLightWall.ogg");
		singleton->soundList[SOUNDS::soundEffectYouDied].loadFromFile("GameFiles/Sound/soundEffectYouDied.ogg");
		singleton->soundList[SOUNDS::soundEffectHunterJump].loadFromFile("GameFiles/Sound/soundEffectHunterJump.ogg");
		singleton->soundList[SOUNDS::soundEffectAlarm].loadFromFile("GameFiles/Sound/soundEffectAlarm.ogg");
		singleton->soundList[SOUNDS::soundEffectFlies].loadFromFile("GameFiles/Sound/soundEffectFlies.ogg");
		singleton->soundList[SOUNDS::soundEffectFrogs].loadFromFile("GameFiles/Sound/soundEffectFrogs.ogg");
		singleton->soundList[SOUNDS::soundEffectNeonSign].loadFromFile("GameFiles/Sound/soundEffectNeonSign.ogg");
		singleton->soundList[SOUNDS::soundEffectClusterlingExplosion].loadFromFile("GameFiles/Sound/soundEffectClusterlingExplosion.ogg");
		singleton->soundList[SOUNDS::soundEffectCrows].loadFromFile("GameFiles/Sound/soundEffectCrows.ogg");

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

int SoundPlayer::playJump(int role, float x, float y, float z)
{
	if (role == 0)
	{
			GetSound()->playExternalSound(SOUNDS::soundEffectTrapperJump, x, y, z);
	}

	else if (role == 2)
	{
		GetSound()->playExternalSound(SOUNDS::soundEffectHunterJump, x, y, z);
	}
	else
	{
		return 0;
	}
}

int SoundPlayer::playMapSounds()
{
	{
		if (soundEnabler == 1 && initialized == 1)
		{

			/*if (musicPlayer.getStatus() == sf::Sound::Playing)
			{
				theCantinaSong.stop();
			}*/

			if (!theCantinaSong.openFromFile(musicList[cantinaSong]))
			{
				return -1;
			}
			theCantinaSong.setPosition(44, 1.55, 100);
			theCantinaSong.setAttenuation(50);
			theCantinaSong.setVolume(20);
			theCantinaSong.play();
			theCantinaSong.setLoop(true);
			theCantinaSong.setMinDistance(10);
			
			mapSounds[0].setBuffer(soundList[SOUNDS::soundAids]);
			mapSounds[0].setPosition(-10, 5, -10);
			mapSounds[0].setVolume(30);
			mapSounds[0].setAttenuation(10);
			mapSounds[0].setMinDistance(5.0f);
			mapSounds[1].setBuffer(soundList[SOUNDS::power]);
			mapSounds[1].setPosition(-10, 1.55, -50);
			mapSounds[1].setVolume(30);
			mapSounds[1].setAttenuation(10);
			mapSounds[1].setMinDistance(5.0f);
			mapSounds[2].setBuffer(soundList[SOUNDS::soundEffectAlarm]);
			mapSounds[2].setPosition(73, 1.55, 4);
			mapSounds[2].setVolume(20);
			mapSounds[2].setAttenuation(20);
			mapSounds[2].setMinDistance(50.0f);
			mapSounds[3].setBuffer(soundList[SOUNDS::soundEffectFlies]);
			mapSounds[3].setPosition(51, 1.55, 108);
			mapSounds[3].setVolume(30);
			mapSounds[3].setAttenuation(3);
			mapSounds[3].setMinDistance(9.0f);
			mapSounds[4].setBuffer(soundList[SOUNDS::soundEffectFlies]);
			mapSounds[4].setPosition(-74, 1.55, 30);
			mapSounds[4].setVolume(30);
			mapSounds[4].setAttenuation(3);
			mapSounds[4].setMinDistance(9.0f);
			mapSounds[5].setBuffer(soundList[SOUNDS::soundEffectFrogs]);
			mapSounds[5].setPosition(-48, 1.55, 72);
			mapSounds[5].setVolume(10);
			mapSounds[5].setAttenuation(10);
			mapSounds[5].setMinDistance(4.0f);
			mapSounds[6].setBuffer(soundList[SOUNDS::soundEffectCrows]);
			mapSounds[6].setPosition(35, 1.55, 15);
			mapSounds[6].setVolume(20);
			mapSounds[6].setAttenuation(20);
			mapSounds[6].setMinDistance(35.0f);
			mapSounds[7].setBuffer(soundList[SOUNDS::soundEffectFlies]);
			mapSounds[7].setPosition(-32, 1.55, 47);
			mapSounds[7].setVolume(30);
			mapSounds[7].setAttenuation(3);
			mapSounds[7].setMinDistance(9.0f);

			for (int i = 0; i < MAXSOUNDS; i++)
			{
				mapSounds[i].setLoop(true);
				mapSounds[i].play();
			}
		}

		return 0;
	}

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

int SoundPlayer::playDestroyer(float x, float y, float z)
{
	
	if (soundEnabler && initialized == 1)
	{
		playDestroyerStart(x, y, z);
		if (sf::Listener::getPosition().x > x - 0.1 && sf::Listener::getPosition().x < x + 0.1)
		{
			if (sf::Listener::getPosition().z > z - 0.1 && sf::Listener::getPosition().z < z + 0.1)
			{
				destroyerSteps.setRelativeToListener(true);
				destroyerSteps.setPosition(0, 0, 0);

			}
		}

		else
		{
			destroyerSteps.setRelativeToListener(false);
			destroyerSteps.setPosition(x, y, z);			//Set the sound's position in the world. Could be passed in through a parameter.
		}

		//std::cout << "x: " << sf::Listener::getPosition().x << " y: " << sf::Listener::getPosition().y << " z: " << sf::Listener::getPosition().z << endl;
		//std::cout << "x: " << x << " y: " << y << " z: " << z << endl;
		//sounds[nrOfSoundsPlaying].isRelativeToListener();
		destroyerSteps.setMinDistance(10.0f);		//Set the sound's distance it travels before it starts to attenuate. Could be passed in through a parameter.

		destroyerSteps.setBuffer(soundList[SOUNDS::soundEffectDestroyerSteps]);
		destroyerSteps.setVolume(50);
		destroyerSteps.setLoop(true);
		destroyerSteps.play();
	}

	return 0;
}

int SoundPlayer::playDestroyerStop(float x, float y, float z)
{

	if (soundEnabler && initialized == 1)
	{
		if (sf::Listener::getPosition().x > x - 0.1 && sf::Listener::getPosition().x < x + 0.1)
		{
			if (sf::Listener::getPosition().z > z - 0.1 && sf::Listener::getPosition().z < z + 0.1)
			{
				destroyerStop.setRelativeToListener(true);
				destroyerStop.setPosition(0, 0, 0);

			}
		}

		else
		{
			destroyerStop.setRelativeToListener(false);
			destroyerStop.setPosition(x, y, z);			//Set the sound's position in the world. Could be passed in through a parameter.
		}

		//std::cout << "x: " << sf::Listener::getPosition().x << " y: " << sf::Listener::getPosition().y << " z: " << sf::Listener::getPosition().z << endl;
		//std::cout << "x: " << x << " y: " << y << " z: " << z << endl;
		//sounds[nrOfSoundsPlaying].isRelativeToListener();
		destroyerStop.setMinDistance(10.0f);		//Set the sound's distance it travels before it starts to attenuate. Could be passed in through a parameter.

		destroyerStop.setBuffer(soundList[SOUNDS::soundEffectDestroyerStop]);
		destroyerStop.setVolume(50);
		destroyerStop.setLoop(false);
		destroyerStop.play();
	}

	return 0;
}

int SoundPlayer::playDestroyerStart(float x, float y, float z)
{

	if (soundEnabler && initialized == 1)
	{
		if (sf::Listener::getPosition().x > x - 0.1 && sf::Listener::getPosition().x < x + 0.1)
		{
			if (sf::Listener::getPosition().z > z - 0.1 && sf::Listener::getPosition().z < z + 0.1)
			{
				destroyerStart.setRelativeToListener(true);
				destroyerStart.setPosition(0, 0, 0);

			}
		}

		else
		{
			destroyerStart.setRelativeToListener(false);
			destroyerStart.setPosition(x, y, z);			//Set the sound's position in the world. Could be passed in through a parameter.
		}

		//std::cout << "x: " << sf::Listener::getPosition().x << " y: " << sf::Listener::getPosition().y << " z: " << sf::Listener::getPosition().z << endl;
		//std::cout << "x: " << x << " y: " << y << " z: " << z << endl;
		//sounds[nrOfSoundsPlaying].isRelativeToListener();
		destroyerStop.setMinDistance(10.0f);		//Set the sound's distance it travels before it starts to attenuate. Could be passed in through a parameter.

		destroyerStart.setBuffer(soundList[SOUNDS::soundEffectDestroyerStart]);
		destroyerStart.setVolume(50);
		destroyerStart.setLoop(false);
		destroyerStart.play();
	}

	return 0;
}

int SoundPlayer::playExternalSound(int sound, float x, float y, float z)
{
	if (soundEnabler && initialized == 1)
	{
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
	sf::Listener::setDirection(playerDir.x, 0.0, playerDir.z);
}

void SoundPlayer::setLocalPlayerPos(glm::vec3 playerPos)
{
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

		//musicPlayer.play();
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

void SoundPlayer::playFootsteps(int role, float posX, float posY, float posZ)
{
	
		
		if (role == 0)
		{
			playExternalSound(SOUNDS::soundFootSteps, posX, posY, posZ);
		}

		if (role == 1)
		{
			playDestroyer(posX, posY, posZ);
		}

		if (role == 2)
		{
			playExternalSound(SOUNDS::soundEffectShankerSteps, posX, posY, posZ);
		}

		if (role == 3)
		{
			playExternalSound(SOUNDS::soundEffectBruteSteps, posX, posY, posZ);
		}

		if (role == 4)
		{
			playExternalSound(SOUNDS::soundStepsManipulator, posX, posY, posZ);
		}
	
	
}

