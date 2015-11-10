#ifndef SOUND_H
#define SOUND_H
#include<sfml/Audio.hpp>

class SoundPlayer
{
private:
	sf::Sound soundPlayer;	//Loads small sound files into the memory
	sf::Music musicPlayer;	//Streams big sound files

	sf::SoundBuffer soundBuffer;

public:
	SoundPlayer();
	~SoundPlayer();

	int playSound(int sound);
	int playMusic(int music);
};

#endif