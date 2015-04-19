#include "trident.h"
#include "sprite_util.h"

namespace
{
	sf::IntRect const BBOX(27, 10, 35, 8);
	int const SPRITE_WIDTH = 67, SPRITE_HEIGHT = 29;
}

Trident::Trident(float x, float y, int dir, TextureManager &textureManager)
	: Trident(x, y, BBOX.width, BBOX.height, dir, textureManager)
{
	animationFrames.emplace_back(0, 0, SPRITE_WIDTH, SPRITE_HEIGHT);
	SpriteUtil::setOriginAndScale(sprite, sf::Vector2f(static_cast<float>(dir), 1.0f), BBOX);
}

Trident::Trident(float x, float y, int width, int height, int dir, TextureManager &textureManager)
	: Weapon(x, y, width, height, 0.4f * dir, 0, 0, 0, 10, textureManager)
{
	destroyedOnHit = false;
	setDepth(-4);
	sprite.setTexture(textureManager.getRef("trident"));
}

Trident::~Trident()
{
}

Trident* Trident::spawnAdjusted(float x, float y, int dir, TextureManager &textureManager)
{
	if (dir < 0) x -= BBOX.width;
	return new Trident(x, y, dir, textureManager);
}
