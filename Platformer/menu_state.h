#ifndef MENU_STATE_H
#define MENU_STATE_H

#include <SFML/Audio.hpp>
#include <vector>

#include "settings.h"
#include "state.h"

class Menu_State : public State
{
	private:
		TextureManager const &textureManager;
		settings_t &settings;
		std::vector<std::string> menuOptions;
		sf::Sprite sprLogo;
		sf::Font fnt;
		unsigned int currentOption = 0;
	public:
		Menu_State(StateManager &sM, TextureManager const &textureManager, settings_t &settings);
		virtual ~Menu_State();

		// Actions
		virtual void draw(sf::RenderWindow &window) override;
		virtual void update(sf::RenderWindow &window, SoundManager &soundManager, InputHandler &inputHandler) override;
};

#endif
