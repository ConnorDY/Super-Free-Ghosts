#include "obelisk.h"

Obelisk::Obelisk(TextureManager &textureManager, float x, float y, float height)
	: Object(
	x, y, 30, height
	)
{
	spr.setTexture(textureManager.getRef("obelisk"));
}


/* Actions */
void Obelisk::draw(sf::RenderWindow &window)
{
	spr.setPosition(sf::Vector2f(x, y));
	window.draw(spr);
}
