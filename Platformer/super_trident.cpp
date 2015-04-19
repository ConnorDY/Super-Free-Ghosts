#include <vector>
#include "super_trident.h"
#include "sprite_util.h"

namespace
{
	sf::IntRect const BBOX(27, 9, 36, 10);
	int const SPRITE_WIDTH = 67, SPRITE_HEIGHT = 29;
	int const WATER_SPRITE_TOP = 2 * SPRITE_HEIGHT, TRIDENT_SPRITE_TOP = SPRITE_HEIGHT;
}

SuperTrident::SuperTrident(float x, float y, int dir, TextureManager &textureManager)
	: Trident(x, y, BBOX.width, BBOX.height, dir, textureManager)
{
	for (int i = 0; i < 6; i++)
		animationFrames.emplace_back(i * SPRITE_WIDTH, WATER_SPRITE_TOP, SPRITE_WIDTH, SPRITE_HEIGHT);
	sf::Vector2f scaleVec(static_cast<float>(dir), 1.0f);
	SpriteUtil::setOriginAndScale(sprite, scaleVec, BBOX);

	tridentOverlay.setTexture(textureManager.getRef("trident"));
	tridentOverlay.setTextureRect(sf::IntRect(0, TRIDENT_SPRITE_TOP, SPRITE_WIDTH, SPRITE_HEIGHT));
	tridentOverlay.setColor(sf::Color(255, 255, 255, 128));
	SpriteUtil::setOriginAndScale(tridentOverlay, scaleVec, BBOX);
}

SuperTrident::~SuperTrident()
{
}

void SuperTrident::draw(sf::RenderWindow &window)
{
	// Use default drawing method for animated part (water)
	Trident::draw(window);

	// Draw trident on top
	tridentOverlay.setPosition(x, y);
	window.draw(tridentOverlay);
}

SuperTrident* SuperTrident::spawnAdjusted(float x, float y, int dir, TextureManager &textureManager)
{
	if (dir < 0) x -= BBOX.width;
	return new SuperTrident(x, y, dir, textureManager);
}
