#ifndef BLOCK_H
#define BLOCK_H

#include <SFML/Graphics.hpp>
#include <math.h>

#include "object.h"

class Block : public Object
{
	private:
		sf::RectangleShape rectangle;
		void updateSize();
	public:
		Block();
		Block(float xx, float yy, float sWidth, float sHeight);
		
		// Mutators
		void setWidth(float sWidth);
		void setHeight(float sHeight);

		// Actions
		void draw(sf::RenderWindow &window);
};

#endif
