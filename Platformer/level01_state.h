#ifndef LEVEL01_STATE_H
#define LEVEL01_STATE_H

#include <SFML/Audio.hpp>
#include <algorithm>
#include <vector>

#include "settings.h"
#include "level_state.h"

class Level01_State : public LevelState
{
	private:
		sf::Music music;
		sf::Sprite bg01;
	public:
		Level01_State(StateManager &sM, SoundManager &som, TextureManager &textureManager, settings_t &settings);
		virtual ~Level01_State();

		// Actions
		virtual void start();
		virtual void drawBackground(sf::RenderWindow &window);
		virtual void drawForeground(sf::RenderWindow &window);
};

#endif
