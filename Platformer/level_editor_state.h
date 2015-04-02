#ifndef LEVEL_EDITOR_STATE_H
#define LEVEL_EDITOR_STATE_H

#include <SFML/Audio.hpp>
#include <algorithm>
#include <vector>

#include "globals.h"
#include "settings.h"
#include "player.h"
#include "projectile.h"
#include "zombie.h"
#include "obelisk.h"
#include "block.h"
#include "room.h"

class Level_Editor_State : public Room
{
	private:
		sf::Clock restartTimer;
		sf::Vector2i point;
		bool clicked;	
	public:
		Level_Editor_State(StateManager &sM, SoundManager &som, TextureManager &textureManager, const settings_t &settings);
		virtual ~Level_Editor_State();

		// Actions
		void start(TextureManager &textureManager, const settings_t &settings);
		virtual void update(sf::RenderWindow &window, TextureManager &textureManager, SoundManager &soundManager, InputHandler &inputHandler, settings_t &settings);
};

#endif
