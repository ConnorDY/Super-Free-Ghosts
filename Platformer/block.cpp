#include "block.h"
#include "globals.h"

Block::Block(float x, float y, float width, float height)
	: Object(
			x, y, width, height
	  ),
	  rectangle(sf::Vector2f(width, height))
{
}


/* Actions */
void Block::draw(sf::RenderWindow &window)
{
	rectangle.setPosition(x, y);
	window.draw(rectangle);
}
