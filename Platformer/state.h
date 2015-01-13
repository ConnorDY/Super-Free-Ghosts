#ifndef STATE_H
#define STATE_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include "texture_manager.h"
#include "sound_manager.h"
#include "input_handler.h"
#include "globals.h"

class State
{
	private:
		sf::View view;
		sf::Clock clock;
	public:
		State();

		// Mutators
		void setView(sf::View v);

		// Accessors
		sf::View* getView();
		sf::Clock* getClock();
		float getViewX();

		// Actions
		void draw(sf::RenderWindow &window);
		void update(sf::RenderWindow &window, TextureManager &textureManager, SoundManager &soundManager, InputHandler &inputHandler);
		sf::Time restartClock();
};

#endif
