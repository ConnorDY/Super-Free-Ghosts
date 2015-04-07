#ifndef DIALOGUE_H
#define DIALOGUE_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>
#include <iostream>

#include "globals.h"
#include "input_handler.h"

class Dialogue
{
	private:
		sf::Font fnt;
		sf::Text txt;
		sf::RectangleShape rect;
		sf::Clock timer;
		unsigned int index, line;
		bool done;
		std::vector<std::string> textLines;
	public:
		Dialogue(const std::vector<std::string> lines);
		~Dialogue();

		void setup();
		void draw(sf::RenderWindow &window, const sf::View &view);
		void update(InputHandler &inputHandler);
};

#endif
