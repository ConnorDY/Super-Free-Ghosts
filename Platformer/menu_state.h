#ifndef MENU_STATE_H
#define MENU_STATE_H

#include <SFML/Audio.hpp>
#include <vector>

#include "globals.h"
#include "state.h"

class Menu_State : public State
{
	private:
		std::vector<std::string> menuOptions;
		sf::Music music;
	public:
		Menu_State(StateManager *sM, TextureManager &textureManager);
		~Menu_State();

		// Actions
		void draw(sf::RenderWindow &window);
		void update(sf::RenderWindow &window, TextureManager &textureManager, SoundManager &soundManager, InputHandler &inputHandler);
};

#endif
