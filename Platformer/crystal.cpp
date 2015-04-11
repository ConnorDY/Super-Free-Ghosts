#include "crystal.h"
#include "player.h"

Crystal::Crystal(TextureManager &textureManager, float x, float y)
	: Object(
	x, y, 15, 15,
	0.0f, 0.0f, false
	)
{
	setDepth(-3);
	rect.setSize(sf::Vector2f(width, height));
	rect.setFillColor(sf::Color(0, 70, 255, 200));
}

Crystal::~Crystal()
{
}

void Crystal::draw(sf::RenderWindow &window)
{
	rect.setPosition(x, y);
	window.draw(rect);
}

void Crystal::update(sf::Time deltaTime, Room &room, const settings_t &settings)
{
	for (auto col : allCollisions(x, y, room))
	{
		Player* player = dynamic_cast<Player*>(col);
		if (player != nullptr)
		{
			player->upgrade(1);
			kill(room, settings);
		}
	}
}