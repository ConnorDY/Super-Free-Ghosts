#ifndef BLOCK_H
#define BLOCK_H

#include <SFML/Graphics.hpp>
#include <math.h>

#include "object.h"

class Block : public Object
{
	private:
		sf::RectangleShape rectangle;
	public:
		explicit Block(Room &room, float x = -128, float y = -128, float width = 32, float height = 32);

		// Actions
		virtual void draw(sf::RenderWindow &window) override;
};

#endif
