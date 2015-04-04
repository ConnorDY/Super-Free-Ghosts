#include "chest.h"

Chest::Chest(TextureManager &textureManager, float x, float y)
	: Object(
	x, y, 20, 17
	)
{
	rect.setSize(sf::Vector2f(width, height));

	health = 3;
}

void Chest::damage(const Room &room, const settings_t &settings)
{
	if (health > 1) health--;
	else kill(room, settings);

	std::cout << "Hit! Health: " << std::to_string(health) << std::endl;
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