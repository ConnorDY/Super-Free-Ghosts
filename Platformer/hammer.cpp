#include "hammer.h"
#include "sprite_util.h"

namespace {
	sf::IntRect const BBOX(3, 3, 13, 13);
	int const SPRITE_WIDTH = 19, SPRITE_HEIGHT = 19;
}

Hammer::Hammer(float x, float y, int dir, TextureManager &textureManager)
	: Hammer(x, y, BBOX.width, BBOX.height, dir, textureManager)
{
	for (int i = 0; i < 6; i++)
		animationFrames.emplace_back(i * SPRITE_WIDTH, 0, SPRITE_WIDTH, SPRITE_HEIGHT);
	SpriteUtil::setOriginAndScale(sprite, sf::Vector2f(static_cast<float>(dir), 1.0f), BBOX);
}

Hammer::Hammer(float x, float y, int width, int height, int dir, TextureManager &textureManager)
	: Weapon(x, y, width, height, 0.15f * dir, -0.3f, 0.0007f, 0.4f, 10, textureManager)
{
	setDepth(-4);
	sprite.setTexture(textureManager.getRef("hammer"));
}

Hammer::~Hammer()
{
}

Hammer* Hammer::spawnAdjusted(float x, float y, int dir, TextureManager &textureManager)
{
	if (dir < 0) x -= BBOX.width;
	return new Hammer(x, y, dir, textureManager);
}
