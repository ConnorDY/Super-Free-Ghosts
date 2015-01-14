#include "block.h"

Block::Block(TextureManager &textureManager, float x, float y, float width, float height)
	: Object(
			Object::Type::Block,
			x, y, width, height
	  )
{
	sprite.setTexture(textureManager.getRef("tiles"));
	sprite.setScale(sf::Vector2f(2.0f, 2.0f));

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
	for (unsigned int i = 0; i < (unsigned int)(getWidth() / 32.0f); i++)
	{
		// Top of ground with grass
		sprite.setTextureRect(sf::IntRect(0, 0, 16, 38));
		sprite.setOrigin(sf::Vector2f(0.0f, 13.0f));
		sprite.setPosition(roundf(getX()) + (i * 32.0f), roundf(getY()));
		window.draw(sprite);

		// Ground underneath
		for (float j = 0; getY() + 38.0f + j < getY() + getHeight(); j++)
		{
			sprite.setTextureRect(sf::IntRect(0, 38, 16, 16));
			sprite.setOrigin(sf::Vector2f(0.0f, 0.0f));
			sprite.setPosition(roundf(getX()) + (i * 32.0f), roundf(getY()) + 38.0f + (j * 32.0f));
			window.draw(sprite);
		}
	}

	if (DEBUG_MODE)
	{
		rectangle.setPosition(roundf(getX()), roundf(getY()));
		window.draw(rectangle);
	}
}

void Block::updateSize()
{
	rectangle.setSize(sf::Vector2f(getWidth(), getHeight()));
}
