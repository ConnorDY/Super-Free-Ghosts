#include "flame.h"
#include "room.h"
#include "assert.h"

namespace
{
	sf::IntRect const BBOX(0, 0, 8, 8);
	int const SPRITE_WIDTH = 0, SPRITE_HEIGHT = 0;
}

Flame::Flame(Room &room, float x, float y, int dir, int remaining)
	: Weapon(room, x, y, BBOX.width, BBOX.height, 0, 0, 0, 0, 5),
		remaining(remaining), lifetime(100), dir(dir)
{
	rectangle.setFillColor(sf::Color(255,0,0,128));
	// TODO: Set sprite and such
}

Flame::~Flame() {}

void Flame::move(sf::Time deltaTime)
{
	auto static const maxYAdjust = 10;

	// If we're in the air, go down (this is not an animation but a jump, intended for before drawing after spawn or a heightmap change)
	if (placeFree(x, y + 1)) y += maxYAdjust;

	auto oldY = y;
	pushOutOfSolids();
	// If we pushed too far up, kill ourselves (because we are too far down in a wall and should not jump to its top)
	if (oldY - y > maxYAdjust) del = true;  // Do not call onDeath(), do not want a respawn here

	// Same goes for if we're still in the air despite the above
	if (placeFree(x, y + 1)) del = true;

	lifetime -= deltaTime.asMilliseconds();
	if (lifetime <= 0) kill();
}

void Flame::onDeath()
{
	if (remaining > 0)
		room.spawn(new Flame(room, x + 6 * dir, y, dir, remaining - 1));
}
