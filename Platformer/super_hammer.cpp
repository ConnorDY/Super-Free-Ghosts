#include <vector>
#include "super_hammer.h"
#include "sprite_util.h"
#include "room.h"

namespace {
	sf::IntRect const BBOX(3, 3, 13, 13);
	int const SPRITE_WIDTH = 19, SPRITE_HEIGHT = 19;
}

SuperHammer::SuperHammer(float x, float y, int dir, TextureManager &textureManager)
	: SuperHammer(x, y, dir, 1, textureManager)
{
}

SuperHammer::SuperHammer(float x, float y, int dir, int remainingBounces, TextureManager &textureManager)
	: Hammer(x, y, BBOX.width, BBOX.height, dir, textureManager),
	  remainingBounces(remainingBounces)
{
	for (int i = 0; i < 6; i++)
		animationFrames.emplace_back(i * SPRITE_WIDTH, SPRITE_HEIGHT, SPRITE_WIDTH, SPRITE_HEIGHT);
	SpriteUtil::setOriginAndScale(sprite, sf::Vector2f(static_cast<float>(dir), 1.0f), BBOX);
}

SuperHammer::~SuperHammer()
{
}

void SuperHammer::move(sf::Time deltaTime, Room &room, settings_t const &settings)
{
	Hammer::move(deltaTime, room, settings);

	// If we hit something, manually kill the hammer BUT undo our last vertical movement, just in case we hit the ground
	// This y-value will then be used in onDeath
	if (!placeFree(x, y, room))
	{
		float const mstime = deltaTime.asMicroseconds() / 1000.0f;
		y -= dy * mstime;
		kill(room, settings);
	}
}


SuperHammer* SuperHammer::spawnAdjusted(float x, float y, int dir, TextureManager &textureManager)
{
	if (dir < 0) x -= BBOX.width;
	return new SuperHammer(x, y, dir, textureManager);
}

void SuperHammer::onDeath(Room &room, settings_t const&)
{
	if (remainingBounces != 0)
	{
		auto bouncedHammer = new SuperHammer(x, y, dx > 0 ? 1 : -1, remainingBounces - 1, room.textureManager);

		// Make the hammer go up, if it wasn't already
		if (dy > 0)
			dy = -dy / 2.0f;

		bouncedHammer->dy = dy;
		room.spawn(bouncedHammer);
	}
}
