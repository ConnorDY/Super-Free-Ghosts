#include "flame.h"
#include "room.h"

Flame::Flame(float x, float y, int remaining, TextureManager &textureManager)
	: Weapon(x, y, 8, 8, 0, 0, 0, 0, textureManager),
	  remaining(remaining), lifetime(300)
{
}

Flame::~Flame() {}

void Flame::move(sf::Time deltaTime, Room &room, settings_t const &settings)
{
	auto oldY = y;
	pushOutOfSolids(room);
	if (oldY - y > 10) kill(room, settings);

	lifetime -= deltaTime.asMilliseconds();
	if (lifetime <= 0) kill(room, settings);
}

void Flame::onDeath(Room &room, settings_t const&)
{
	room.spawn(new Flame(x + 4, y, remaining - 1, room.textureManager));
}
