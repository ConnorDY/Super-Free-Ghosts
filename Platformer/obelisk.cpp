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
	spr.setPosition(sf::Vector2f(x, y - 1)); // "y - 1" as an easy collision fix of sorts
	window.draw(spr);
}
