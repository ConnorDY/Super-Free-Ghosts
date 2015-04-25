#include "spear.h"
#include "room.h"
#include "sprite_util.h"

namespace {
	// Normal spear: Sprite strip of 49x12 sprites, 1 frame, BB 35x6+8+3
	sf::IntRect const BBOX(8, 3, 35, 6);
	int const SPRITE_WIDTH = 49, SPRITE_HEIGHT = 12;
}

Spear::Spear(Room &room, float x, float y, int dir)
	: Spear(room, x, y, BBOX.width, BBOX.height, dir)
{
	animationFrames.emplace_back(0, 0, SPRITE_WIDTH, SPRITE_HEIGHT);
	SpriteUtil::setOriginAndScale(sprite, sf::Vector2f(static_cast<float>(dir), 1.0f), BBOX);
}

Spear::Spear(Room &room, float x, float y, int width, int height, int dir)
	: Weapon(room, x, y, width, height, 0.4f * dir, 0, 0, 0, 8)
{
	setDepth(-4);
	sprite.setTexture(room.textureManager.getRef("spear"));
}

Spear::~Spear()
{
}

Spear* Spear::spawnAdjusted(Room &room, float x, float y, int dir)
{
	if (dir < 0) x -= BBOX.width;
	return new Spear(room, x, y, dir);
}
