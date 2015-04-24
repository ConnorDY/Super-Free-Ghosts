#include "block.h"
#include "globals.h"

Block::Block(Room &room, float x, float y, float width, float height)
	: Object(
			room, x, y, width, height
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
