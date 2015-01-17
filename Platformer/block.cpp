#include "block.h"

Block::Block(TextureManager &textureManager, float x, float y, float width, float height)
	: Object(
			Object::Type::Block,
			x, y, width, height
	  ),
	  rectangle(sf::Vector2f(width, height))
{
	sprite.setTexture(textureManager.getRef("tiles"));
}


/* Actions */
void Block::draw(sf::RenderWindow &window)
{
	for (unsigned int i = 0; i < (unsigned int)(width / 16.0f); i++)
	{
		// Top of ground with grass
		sprite.setTextureRect(sf::IntRect(0, 0, 16, 38));
		sprite.setOrigin(sf::Vector2f(0.0f, 13.0f));
		sprite.setPosition(x + (i * 16.0f), y);
		window.draw(sprite);

		// Ground underneath
		for (float j = 0; y + 19.0f + j < y + height; j++)
		{
			sprite.setTextureRect(sf::IntRect(0, 38, 16, 16));
			sprite.setOrigin(sf::Vector2f(0.0f, 0.0f));
			sprite.setPosition(x + (i * 16.0f), y + 19.0f + (j * 16.0f));
			window.draw(sprite);
		}
	}

	if (DEBUG_MODE)
	{
		rectangle.setPosition(x, y);
		window.draw(rectangle);
	}
}
