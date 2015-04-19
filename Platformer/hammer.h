#ifndef HAMMER_H
#define HAMMER_H

#include "weapon.h"

class Hammer : public Weapon
{
	protected:
		// See Spear for what this constructor does
		Hammer(float x, float y, int width, int height, int dir, TextureManager &textureManager);
	public:
		Hammer(float x, float y, int dir, TextureManager &textureManager);
		virtual ~Hammer();
		static Hammer* spawnAdjusted(float x, float y, int dir, TextureManager &textureManager);
};

#endif
