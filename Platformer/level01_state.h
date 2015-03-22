#ifndef LEVEL01_STATE_H
#define LEVEL01_STATE_H

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

class Level01_State : public Room
{
	private:
		Player *player;
		sf::Music music;
		sf::Sprite bg01, bg02, bg03, under01;

		void drawTree(int x, int y, sf::RenderWindow &window);
		void drawDecor(int x, int y, int type, sf::RenderWindow &window);
		
		int countZombies();
	public:
		Level01_State(StateManager &sM, SoundManager &som, TextureManager &textureManager, const settings_t &settings);
		virtual ~Level01_State();

		// Actions
		void start(TextureManager &textureManager, const settings_t &settings);
		virtual void drawBackground(sf::RenderWindow &window);
		virtual void drawForeground(sf::RenderWindow &window);
		virtual void update(sf::RenderWindow &window, TextureManager &textureManager, SoundManager &soundManager, InputHandler &inputHandler, settings_t &settings);
};

#endif
