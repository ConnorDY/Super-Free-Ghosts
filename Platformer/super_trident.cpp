#include <vector>
#include "super_trident.h"
#include "sprite_util.h"

namespace
{
	sf::IntRect const BBOX(27, 9, 36, 10);
	int const SPRITE_WIDTH = 67, SPRITE_HEIGHT = 29;
}

SuperTrident::SuperTrident(float x, float y, int dir, TextureManager &textureManager)
	: Trident(x, y, BBOX.width, BBOX.height, dir, textureManager)
{
	animationFrames.emplace_back(0, SPRITE_HEIGHT, SPRITE_WIDTH, SPRITE_HEIGHT);
	SpriteUtil::setOriginAndScale(sprite, sf::Vector2f(static_cast<float>(dir), 1.0f), BBOX);
}

SuperTrident::~SuperTrident()
{
}

SuperTrident* SuperTrident::spawnAdjusted(float x, float y, int dir, TextureManager &textureManager)
{
	if (dir < 0) x -= BBOX.width;
	return new SuperTrident(x, y, dir, textureManager);
}
