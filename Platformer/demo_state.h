#ifndef DEMO_STATE_H
#define DEMO_STATE_H

#include <SFML/Audio.hpp>
#include <algorithm>
#include <vector>

#include "settings.h"
#include "player.h"
#include "dialogue.h"
#include "room.h"

class Demo_State : public Room
{
	private:
		Player *player;
		Dialogue *dialogue;
		sf::Clock restartTimer;
		bool restart;
	public:
		Demo_State(StateManager &sM, SoundManager &som, TextureManager &textureManager, const settings_t &settings);
		virtual ~Demo_State();

		// Actions
		void start(TextureManager &textureManager, const settings_t &settings);
		virtual void drawBackground(sf::RenderWindow &window);
		virtual void drawForeground(sf::RenderWindow &window);
		virtual void update(sf::RenderWindow &window, TextureManager &textureManager, SoundManager &soundManager, InputHandler &inputHandler, settings_t &settings);
};

#endif
