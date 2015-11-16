#ifndef SOUND_H
#define SOUND_H
#include<sfml/Audio.hpp>
#include<glm/glm.hpp>
#include<vector>
#include"SoundList.h"
#include "../Input.h"

using namespace std;

class SoundPlayer
{
private:
	bool soundEnabler;
	int nrOfSoundsPlaying;
	float soundVolume;
	float musicVolume;
	
	sf::Sound sounds[30];
	sf::SoundBuffer soundList[SOUNDS::nrOfSounds];
	string musicList[MUSIC::nrOfMusic];

	sf::Music musicPlayer;	//Streams big sound files

	sf::SoundBuffer soundBuffer;

public:
	SoundPlayer();
	~SoundPlayer();

	void enableSounds();

	void setVolumeMusic(float volume);
	void setVolumeSound(float volume);

	int playMusic(int music);

	int playExternalSound(int sound, sf::Vector3f soundOrigin);
	int playUserGeneratedSound(int sound);

	void rotate(float deltaTime);

	void update();
};

#endif