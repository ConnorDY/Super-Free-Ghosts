#include "hammer.h"
#include "room.h"
#include "sprite_util.h"

namespace {
	sf::IntRect const BBOX(3, 3, 13, 13);
	int const SPRITE_WIDTH = 19, SPRITE_HEIGHT = 19;
}

Hammer::Hammer(Room &room, float x, float y, int dir)
	: Hammer(room, x, y, BBOX.width, BBOX.height, dir)
{
	for (int i = 0; i < 6; i++)
		animationFrames.emplace_back(i * SPRITE_WIDTH, 0, SPRITE_WIDTH, SPRITE_HEIGHT);
	SpriteUtil::setOriginAndScale(sprite, sf::Vector2f(static_cast<float>(dir), 1.0f), BBOX);
}

Hammer::Hammer(Room &room, float x, float y, int width, int height, int dir)
	: Weapon(room, x, y, width, height, 0.14f * dir, -0.23f, 0.0007f, 0.4f, 9)
{
	setDepth(-4);
	sprite.setTexture(room.textureManager.getRef("hammer"));
}

Hammer::~Hammer()
{
}

Hammer* Hammer::spawnAdjusted(Room &room, float x, float y, int dir)
{
	if (dir < 0) x -= BBOX.width;
	return new Hammer(room, x, y, dir);
}
