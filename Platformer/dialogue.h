#ifndef DIALOGUE_H
#define DIALOGUE_H

#include <string>
#include <iostream>

#include "input_handler.h"

class Dialogue
{
	private:
		sf::Font fnt;
		sf::Text txt;
		sf::RectangleShape rect, ip;
		sf::Clock timer, ipTimer;
		unsigned int index, line;
		bool done, ipShow;
		std::vector<std::string> textLines;
	protected:
		virtual void onFinish();
	public:
		Dialogue(const std::vector<std::string> lines);
		~Dialogue();

		bool isDone() const;

		void setup();
		void draw(sf::RenderWindow &window, const sf::View &view);
		void update(InputHandler &inputHandler);
};

#endif
