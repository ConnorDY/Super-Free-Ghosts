#include "armourdrop.h"

ArmourDrop::ArmourDrop(Room &room, float x, float y)
	: Object(
		room, x, y, 15, 15,
		0.0f, 0.0f, false
	)
{
	setDepth(-3);
	rect.setSize(sf::Vector2f(width, height));
	rect.setFillColor(sf::Color(0, 70, 255, 200));

	type = PlayerArmour::GOLD;
}

ArmourDrop::~ArmourDrop()
{
}

void ArmourDrop::draw(sf::RenderWindow &window)
{
	rect.setPosition(x, y);
	window.draw(rect);
}

void ArmourDrop::update(sf::Time)
{
	for (auto col : allCollisions(x, y))
	{
		Player* player = dynamic_cast<Player*>(col);
		if (player != nullptr)
		{
			player->upgrade(type);
			kill();
			break;
		}
	}
}
