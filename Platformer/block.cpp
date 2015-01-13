#include "block.h"

Block::Block()
{
	Block(-128.0f, -128.0f, 32.0f, 32.0f);
}

Block::Block(float xx, float yy, float sWidth, float sHeight)
{
	setX(xx);
	setY(yy);
	
	setWidth(sWidth);
	setHeight(sHeight);
	setSolid(true);
	setType(Object::Type::Block);

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
