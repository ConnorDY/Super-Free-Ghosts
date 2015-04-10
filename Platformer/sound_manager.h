#ifndef SOUND_MANAGER_H
#define SOUND_MANAGER_H

#include <SFML/Audio.hpp>
#include <iostream>
#include <string>
#include <map>

class SoundManager
{
	private:
		std::map<std::string, sf::SoundBuffer*> buffers;
		std::map<std::string, sf::Sound*> sounds;
	public:
		SoundManager();
		~SoundManager();
		SoundManager(SoundManager const &that) = delete;

		// Actions
		void loadSound(const std::string &name, const std::string &filename);
		void playSound(const std::string &sound);
};

#endif
