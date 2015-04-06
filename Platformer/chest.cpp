#include "chest.h"
#include "obelisk.h"

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
	// Don't bother drawing if it's completely submerged
	if (slideUpFraction == 0) return;

	auto bbox = getRect();
	rect.setPosition(sf::Vector2f(bbox.left, bbox.top));
	window.draw(rect);
}

void Chest::update(sf::Time deltaTime, Room const &room, const settings_t &)
{
	if (slidingUp)
	{
		// Our Y value matters now, so check it:
		if (room.heightmapIntersects(sf::FloatRect(x, y, width, height)))
			y = room.getMinTerrainYBetween(x, x + width) - height;
		// Check for obelisk collisions and jump to the top of them
		auto mybbox = getRect();
		for (auto obj : allCollisions(x, y - 1, room))
			if (dynamic_cast<Obelisk*>(obj) != nullptr)
			{
				auto objbbox = obj->getRect();
				y = std::min<int>(y, objbbox.top - mybbox.height);
			}

		// Continue slide up animation
		slideUpFraction = std::min<float>(1, slideUpFraction + deltaTime.asMilliseconds() / 500.f);
		if (slideUpFraction == 1) slidingUp = false;
	}
	// XXX The usual updates are a waste of time for us atm
	//Object::update(deltaTime, room, settings);
}

void Chest::onDoubleJumpedOver(Room const &)
{
	slidingUp = true;
}
