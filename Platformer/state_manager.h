#ifndef STATE_MANAGER_H
#define STATE_MANAGER_H

#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include <memory>

#include "settings.h"
#include "input_handler.h"
#include "texture_manager.h"
#include "sound_manager.h"

class State;

class StateManager
{
	private:
		TextureManager &textureManager;
		SoundManager &soundManager;
		InputHandler inputHandler;
		settings_t &settings;
		std::unique_ptr<State> currentState;
	public:
		StateManager(TextureManager &tM, SoundManager &sM, settings_t &settings);
		~StateManager();

		// Actions
		void draw(sf::RenderWindow &window);
		void update(sf::RenderWindow &window);
		void setState(std::unique_ptr<State> state);
};

#endif
