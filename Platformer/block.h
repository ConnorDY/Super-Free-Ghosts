#ifndef BLOCK_H
#define BLOCK_H

#include <SFML/Graphics.hpp>
#include <math.h>

#include "object.h"

class Block : public Object
{
	private:
		sf::RectangleShape rectangle;
		sf::Sprite sprite;
		void updateSize();
	public:
		explicit Block(TextureManager &textureManager, float x = -128, float y = -128, float width = 32, float height = 32);

		// Mutators
		virtual void setWidth(float sWidth);
		virtual void setHeight(float sHeight);

		// Actions
		virtual void draw(sf::RenderWindow &window);
};

#endif
