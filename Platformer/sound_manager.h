#ifndef SOUND_MANAGER_H
#define SOUND_MANAGER_H

#include <SFML/Audio.hpp>
#include <iostream>
#include <string>
#include <map>
#include "settings.h"

class SoundManager
{
	private:
		settings_t const &settings;
		std::map<std::string, sf::SoundBuffer*> buffers;
		std::map<std::string, sf::Sound*> sounds;
	public:
		SoundManager(settings_t const &settings);
		~SoundManager();
		SoundManager(SoundManager const &that) = delete;

		// Actions
		void loadSound(const std::string &name, const std::string &filename);
		void playSound(const std::string &sound) const;
};

#endif
