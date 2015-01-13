#ifndef STATE_MANAGER_H
#define STATE_MANAGER_H

#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include <memory>

#include "globals.h"
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
		std::unique_ptr<State> currentState;
	public:
		StateManager(TextureManager &tM, SoundManager &sM);
		~StateManager();

		// Actions
		void draw(sf::RenderWindow &window);
		void update(sf::RenderWindow &window);
		void setState(std::unique_ptr<State> state);
};

#endif
