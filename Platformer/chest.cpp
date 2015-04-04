#include "chest.h"

Chest::Chest(TextureManager &textureManager, float x, float y)
	: DamageableObject(
	x, y, 20, 17
	)
{
	rect.setSize(sf::Vector2f(width, height));
	setHealth(3);
}

void Chest::draw(sf::RenderWindow &window)
{
	rect.setPosition(sf::Vector2f(x, y));
	window.draw(rect);
}

void Chest::update(sf::Time deltaTime, Room const &room, const settings_t &settings)
{
	Object::update(deltaTime, room, settings);
}