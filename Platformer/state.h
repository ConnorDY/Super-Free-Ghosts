#ifndef STATE_H
#define STATE_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include "state_manager.h"
#include "texture_manager.h"
#include "sound_manager.h"
#include "input_handler.h"
#include "globals.h"

class State
{
	private:
		StateManager &stateManager;
		sf::View view;
		sf::Clock clock;
	public:
		explicit State(StateManager &sm);
		virtual ~State();

		// Mutators
		void setView(sf::View v);

		// Accessors
		sf::View& getView();
		sf::Clock& getClock();
		float getViewX();
		StateManager& getStateManager();

		// Actions
		virtual void draw(sf::RenderWindow &window);
		virtual void update(sf::RenderWindow &window, TextureManager &textureManager, SoundManager &soundManager, InputHandler &inputHandler);
		sf::Time restartClock();
};

#endif
