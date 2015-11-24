#ifndef SOUND_H
#define SOUND_H

#ifdef SOUNDPLAYER_EXPORTS
#define SOUNDPLAYER_API __declspec( dllexport)
#else
#define SOUNDPLAYER_API __declspec( dllimport  )
#endif

#include<sfml/Audio.hpp>
#include<vector>
#include"SoundList.h"

using namespace std;

class SoundPlayer
{
private:
	static SoundPlayer* singleton;
	static bool soundEnabler;
	static int nrOfSoundsPlaying;
	static float soundVolume;
	static float musicVolume;
	~SoundPlayer();


	sf::Sound sounds[30];
	sf::SoundBuffer soundList[SOUNDS::nrOfSounds];
	string musicList[MUSIC::nrOfMusic];

	sf::Music musicPlayer;	//Streams big sound files

	sf::SoundBuffer soundBuffer;

public:
	SoundPlayer();

	static void release();
	static void init(SoundPlayer* sound);
	static SoundPlayer* getSound();

	virtual void enableSounds();

	virtual void setVolumeMusic(float volume);
	virtual void setVolumeSound(float volume);

	virtual int playMusic(int music);

	virtual int playExternalSound(int sound, float, float, float);
	virtual int playUserGeneratedSound(int sound);

	virtual void rotate(float deltaTime);

	virtual void update();
};

extern "C" SOUNDPLAYER_API SoundPlayer* CreateSound();

extern "C" SOUNDPLAYER_API SoundPlayer* GetSound();

extern "C" SOUNDPLAYER_API void InitSound(SoundPlayer* sound);

extern "C" SOUNDPLAYER_API void ReleaseSound();


#endif