#include "block.h"

Block::Block(float x, float y, float width, float height)
	: Object(
			Object::Type::Block,
			x, y, width, height
	  )
{
	updateSize();
}


/* Mutators */
void Block::setWidth(float sWidth)
{
	Object::setWidth(sWidth);
	updateSize();
}

void Block::setHeight(float sHeight)
{
	Object::setHeight(sHeight);
	updateSize();
}


/* Actions */
void Block::draw(sf::RenderWindow &window)
{
	rectangle.setPosition(roundf(getX()), roundf(getY()));
	window.draw(rectangle);
}

void Block::updateSize()
{
	rectangle.setSize(sf::Vector2f(getWidth(), getHeight()));
}
