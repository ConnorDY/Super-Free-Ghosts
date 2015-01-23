#ifndef LEVEL01_STATE_H
#define LEVEL01_STATE_H

#include <SFML/Audio.hpp>
#include <vector>

#include "globals.h"
#include "player.h"
#include "projectile.h"
#include "zombie.h"
#include "block.h"
#include "room.h"

class Level01_State : public Room
{
	private:
		Player *player;
		sf::Music music;
		sf::Sprite scanlines;
	public:
		Level01_State(StateManager &sM, SoundManager &som, TextureManager &textureManager);
		virtual ~Level01_State();

		// Actions
		void start(TextureManager &textureManager);
		virtual void draw(sf::RenderWindow &window);
		virtual void update(sf::RenderWindow &window, TextureManager &textureManager, SoundManager &soundManager, InputHandler &inputHandler);
};

#endif
