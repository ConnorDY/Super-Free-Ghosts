#include "chest.h"

Chest::Chest(TextureManager &textureManager, float x, float y)
	: DamageableObject(
		x, y, 20, 17
	),
	  slideUpFraction(0),
	  slidingUp(false)
{
	(void)textureManager; // textureManager is unused but will be used in future
	rect.setSize(sf::Vector2f(width, height));
	setHealth(3);
}

sf::FloatRect Chest::getRect() const
{
	auto yAdjustment = (1 - slideUpFraction) * height;
	return sf::FloatRect(x, y + yAdjustment, width, height);
}

void Chest::draw(sf::RenderWindow &window)
{
	auto bbox = getRect();
	rect.setPosition(sf::Vector2f(bbox.left, bbox.top));
	window.draw(rect);
}

void Chest::update(sf::Time deltaTime, Room const &room, const settings_t &settings)
{
	if (slidingUp)
	{
		slideUpFraction = std::min<float>(1, slideUpFraction + deltaTime.asMilliseconds() / 500.f);
		if (slideUpFraction == 1) slidingUp = false;
	}
	Object::update(deltaTime, room, settings);
}

void Chest::onDoubleJumpedOver(Room const &)
{
	slidingUp = true;
}
