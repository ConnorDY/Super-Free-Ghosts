#ifndef STATE_MANAGER_H
#define STATE_MANAGER_H

#include <SFML/Graphics.hpp>
#include <map>
#include <string>

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
		std::map<std::string, State*> states;
		std::string currentState;
	public:
		StateManager(TextureManager &tM, SoundManager &sM);
		~StateManager();

		// Actions
		void draw(sf::RenderWindow &window);
		void update(sf::RenderWindow &window);
		void loadState(std::string state);
		void unloadState(std::string state);
		void setState(std::string state);
};

#endif
