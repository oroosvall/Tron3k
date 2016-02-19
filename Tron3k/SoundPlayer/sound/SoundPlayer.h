#ifndef SOUND_H
#define SOUND_H

#ifdef SOUNDPLAYER_EXPORTS
#define SOUNDPLAYER_API __declspec( dllexport)
#else
#define SOUNDPLAYER_API __declspec( dllimport  )
#endif

#include<sfml/Audio.hpp>
#include <../glm/glm/glm.hpp>
#include<vector>
#include"SoundList.h"

int const MAXSOUNDS = 200;

using namespace std;

class SoundPlayer
{
private:
	static SoundPlayer* singleton;
	static bool soundEnabler;
	static bool initialized;
	static int nrOfSoundsPlaying;
	static float soundVolume;
	static float musicVolume;
	glm::vec3 playerDir;

	~SoundPlayer();

	bool footstepsLoop = false;
	float footstepsCountdown = 0;

	sf::Sound sounds[MAXSOUNDS];
	sf::Sound mapSounds[13];
	sf::Sound destroyerSteps;
	sf::Sound destroyerStop;
	sf::Sound destroyerStart;
	sf::Sound bruteSteps;
	sf::Sound melee[10];
	
	sf::SoundBuffer soundList[SOUNDS::nrOfSounds];
	string musicList[MUSIC::nrOfMusic];
	bool fading = false;
	

	sf::Music musicPlayer;	//Streams big sound files
	sf::Music theCantinaSong;

	sf::SoundBuffer soundBuffer;

public:
	SoundPlayer();
    bool bruteGroundBool = true;
	static void release();
	static void init(SoundPlayer* sound, int activateSound);
	static SoundPlayer* getSound();
	bool destroyerPaused = true;
	bool brutePaused = true;
	virtual bool getFading()
	{
		return this->fading;
	}

	virtual void enableSounds();
	static bool getSoundEnabler();
	static bool getInitialized();

	virtual void SetFading(bool);
	virtual float getVolumeMusic();
	virtual void setVolumeMusic(float volume);
	virtual void setVolumeSound(float volume);
	virtual void setLocalPlayerDir(glm::vec3 playerDir);
	virtual void setLocalPlayerPos(glm::vec3 playerPos);
	virtual int playMusic(int music);
	virtual void stopMusic();
	virtual void PlayStereoFootsteps(int);
	virtual void PlayStereoJump(int);

	virtual int playMapSounds();

	virtual int playJump(int, float, float, float);
	virtual int playLand(int, float, float, float);
	virtual int playExternalSound(int sound, float posX, float posY, float posZ);
	virtual int playUserGeneratedSound(int sound);
	virtual int playDestroyer(float x, float y, float z);
	virtual int playDestroyerStop(float x, float y, float z);
	virtual int playDestroyerStart(float x, float y, float z);
	virtual int playBrute(float, float, float);
	virtual void playMelee(float, float, float);
	virtual void playMeleeStereo();

	virtual void stopBrute()
	{
		bruteSteps.stop();
		brutePaused = true;
	}

	virtual void setBruteLoopFalse()
	{
		bruteSteps.setLoop(false);
	}

	void stopDestroyer(float x, float y, float z)
	{
		destroyerSteps.pause();
		destroyerPaused = true;
		playDestroyerStop(x, y, z);

	}

	virtual void rotate(float deltaTime);

	virtual void playFootsteps(int role, float posX, float posY, float posZ);

};

extern "C" SOUNDPLAYER_API SoundPlayer* CreateSound();

extern "C" SOUNDPLAYER_API SoundPlayer* GetSound();

extern "C" SOUNDPLAYER_API bool GetSoundActivated();

extern "C" SOUNDPLAYER_API bool GetInitialized();

extern "C" SOUNDPLAYER_API void InitSound(SoundPlayer* sound, int activateSound);

extern "C" SOUNDPLAYER_API void ReleaseSound();


#endif