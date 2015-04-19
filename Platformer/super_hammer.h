#ifndef SUPER_HAMMER_H
#define SUPER_HAMMER_H

#include "hammer.h"

class SuperHammer : public Hammer
{
	private:
		int remainingBounces;
	public:
		SuperHammer(float x, float y, int dir, TextureManager &textureManager);
		virtual ~SuperHammer();
		virtual void move(sf::Time deltaTime, Room &room, settings_t const &settings);
		static SuperHammer* spawnAdjusted(float x, float y, int dir, TextureManager &textureManager);
};

#endif
