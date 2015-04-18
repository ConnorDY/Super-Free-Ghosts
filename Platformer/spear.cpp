#include "spear.h"
#include "sprite_util.h"

namespace {
	sf::IntRect const BBOX(8, 3, 35, 6);
	int const SPRITE_WIDTH = 49, SPRITE_HEIGHT = 12;
}

Spear::Spear(float x, float y, int dir, TextureManager &textureManager)
	: Weapon(x, y, BBOX.width, BBOX.height, 0.4f * dir, 0, 0, 0, 10, textureManager)
{
	// Normal spear: Sprite strip of 49x12 sprites, 1 frame, BB 35x6+8+3
	sprite.setTexture(textureManager.getRef("spear"));
	sprite.setTextureRect(sf::IntRect(0, 0, SPRITE_WIDTH, SPRITE_HEIGHT));
	SpriteUtil::setOriginAndScale(sprite,
			sf::Vector2f(static_cast<float>(dir), 1.0f),
			BBOX
	);

	setDepth(-4);
}

Spear::~Spear()
{
}
