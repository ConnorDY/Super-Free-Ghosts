#include <vector>
#include "super_hammer.h"
#include "sprite_util.h"
#include "room.h"

namespace {
	sf::IntRect const BBOX(3, 3, 13, 13);
	int const SPRITE_WIDTH = 19, SPRITE_HEIGHT = 19;
}

SuperHammer::SuperHammer(Room &room, float x, float y, int dir)
	: SuperHammer(room, x, y, dir, 1)
{
}

SuperHammer::SuperHammer(Room &room, float x, float y, int dir, int remainingBounces)
	: Hammer(room, x, y, BBOX.width, BBOX.height, dir),
	  remainingBounces(remainingBounces)
{
	for (int i = 0; i < 6; i++)
		animationFrames.emplace_back(i * SPRITE_WIDTH, SPRITE_HEIGHT, SPRITE_WIDTH, SPRITE_HEIGHT);
	SpriteUtil::setOriginAndScale(sprite, sf::Vector2f(static_cast<float>(dir), 1.0f), BBOX);
}

SuperHammer::~SuperHammer()
{
}

void SuperHammer::move(sf::Time deltaTime)
{
	Hammer::move(deltaTime);

	// If we hit something, manually kill the hammer BUT undo our last vertical movement, just in case we hit the ground
	// This y-value will then be used in onDeath
	if (!placeFree(x, y))
	{
		float const mstime = deltaTime.asMicroseconds() / 1000.0f;
		y -= dy * mstime;
		kill();
	}
}


SuperHammer* SuperHammer::spawnAdjusted(Room &room, float x, float y, int dir)
{
	if (dir < 0) x -= BBOX.width;
	return new SuperHammer(room, x, y, dir);
}

void SuperHammer::onDeath()
{
	if (remainingBounces != 0)
	{
		auto bouncedHammer = new SuperHammer(room, x, y, dx > 0 ? 1 : -1, remainingBounces - 1);

		// Make the hammer go up, if it wasn't already
		if (dy > 0)
			dy = -dy / 2.0f;

		bouncedHammer->dy = dy;
		room.spawn(bouncedHammer);
	}
}
