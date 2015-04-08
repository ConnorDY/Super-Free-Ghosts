#include "flame.h"
#include "room.h"
#include "assert.h"

Flame::Flame(float x, float y, int dir, int remaining, TextureManager &textureManager)
	: Weapon(x, y, 8, 8, 0, 0, 0, 0, textureManager),
	  remaining(remaining), lifetime(100), dir(dir)
{
	rectangle.setFillColor(sf::Color(255,0,0,128));
}

Flame::~Flame() {}

void Flame::move(sf::Time deltaTime, Room &room, settings_t const &settings)
{
	auto static const maxYAdjust = 10;

	// If we're in the air, go down (this is not an animation but a jump, intended for before drawing after spawn or a heightmap change)
	if (placeFree(x, y + 1, room)) y += maxYAdjust;

	auto oldY = y;
	pushOutOfSolids(room);
	// If we pushed too far up, kill ourselves (because we are too far down in a wall and should not jump to its top)
	if (oldY - y > maxYAdjust) del = true;  // Do not call onDeath(), do not want a respawn here

	// Same goes for if we're still in the air despite the above
	if (placeFree(x, y + 1, room)) del = true;

	lifetime -= deltaTime.asMilliseconds();
	if (lifetime <= 0) kill(room, settings);
}

void Flame::onDeath(Room &room, settings_t const&)
{
	if (remaining > 0)
		room.spawn(new Flame(x + 6 * dir, y, dir, remaining - 1, room.textureManager));
}
