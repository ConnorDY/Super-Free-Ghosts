#include <vector>
#include "super_hammer.h"
#include "sprite_util.h"

namespace {
	sf::IntRect const BBOX(3, 3, 13, 13);
	int const SPRITE_WIDTH = 19, SPRITE_HEIGHT = 19;
}

SuperHammer::SuperHammer(float x, float y, int dir, TextureManager &textureManager)
	: Hammer(x, y, BBOX.width, BBOX.height, dir, textureManager),
	  remainingBounces(1)
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

	// If we hit something, time to rebound or die!
	if (!placeFree(x, y, room))
	{
		if (remainingBounces != 0)
		{
			remainingBounces--;
			float const mstime = deltaTime.asMicroseconds() / 1000.0f;
			// First, undo the last vertical movement
			y -= dy * mstime;
			// Then kick us off the ground again
			dy = -dy / 2.0f;
		}
	}
}

SuperHammer* SuperHammer::spawnAdjusted(float x, float y, int dir, TextureManager &textureManager)
{
	if (dir < 0) x -= BBOX.width;
	return new SuperHammer(x, y, dir, textureManager);
}
