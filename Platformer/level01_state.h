#ifndef LEVEL01_STATE_H
#define LEVEL01_STATE_H

#include <SFML/Audio.hpp>
#include <vector>

#include "globals.h"
#include "state.h"
#include "object.h"
#include "player.h"
#include "projectile.h"
#include "block.h"

class Level01_State : public State
{
	private:
		Player *player;
		std::vector<Object*> objects;
		sf::Music music;
	public:
		Level01_State(StateManager *sM, TextureManager &textureManager);
		~Level01_State();

		// Actions
		void start(TextureManager &textureManager);
		void end();
		void reset(TextureManager &textureManager);
		void draw(sf::RenderWindow &window);
		void update(sf::RenderWindow &window, TextureManager &textureManager, SoundManager &soundManager, InputHandler &inputHandler);
};

#endif
