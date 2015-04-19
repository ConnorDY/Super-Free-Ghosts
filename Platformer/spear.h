#ifndef SPEAR_H
#define SPEAR_H

#include "weapon.h"

class Spear : public Weapon
{
	protected:
		// This constructor does not set the animation frames, nor the sprite origin/scale. The only change it makes to the sprite is to set its texture. Hence it is protected, because nothing outside of the inheritance hierarchy could use it.
		Spear(float x, float y, int width, int height, int dir, TextureManager &textureManager);
	public:
		Spear(float x, float y, int dir, TextureManager &textureManager);
		virtual ~Spear();
		static Spear* spawnAdjusted(float x, float y, int dir, TextureManager &textureManager);
};

#endif
