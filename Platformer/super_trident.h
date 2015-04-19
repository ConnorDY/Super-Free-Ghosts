#ifndef SUPER_TRIDENT_H
#define SUPER_TRIDENT_H

#include "trident.h"

class SuperTrident : public Trident
{
	private:
		sf::Sprite tridentOverlay;
	public:
		SuperTrident(float x, float y, int dir, TextureManager &textureManager);
		virtual ~SuperTrident();
		virtual void draw(sf::RenderWindow &window);
		static SuperTrident* spawnAdjusted(float x, float y, int dir, TextureManager &textureManager);
};

#endif
