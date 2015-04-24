#include <vector>
#include "super_spear.h"
#include "sprite_util.h"

namespace {
	sf::IntRect const BBOX(1, 1, 47, 10);
	int const SPRITE_WIDTH = 49, SPRITE_HEIGHT = 12;
}

SuperSpear::SuperSpear(Room &room, float x, float y, int dir)
	: Spear(room, x, y, BBOX.width, BBOX.height, dir)
{
	for (int i = 0; i < 4; i++)
		animationFrames.emplace_back(i * SPRITE_WIDTH, SPRITE_HEIGHT, SPRITE_WIDTH, SPRITE_HEIGHT);
	SpriteUtil::setOriginAndScale(sprite, sf::Vector2f(static_cast<float>(dir), 1.0f), BBOX);
}

SuperSpear::~SuperSpear()
{
}

SuperSpear* SuperSpear::spawnAdjusted(Room &room, float x, float y, int dir)
{
	if (dir < 0) x -= BBOX.width;
	return new SuperSpear(room, x, y, dir);
}
