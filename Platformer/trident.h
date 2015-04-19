#ifndef TRIDENT_H
#define TRIDENT_H

#include "weapon.h"

class Trident : public Weapon
{
	protected:
		// This constructor does not set the animation frames, nor the sprite origin/scale. The only change it makes to the sprite is to set its texture. Hence it is protected, because nothing outside of the inheritance hierarchy could use it.
		Trident(float x, float y, int width, int height, int dir, TextureManager &textureManager);
	public:
		Trident(float x, float y, int dir, TextureManager &textureManager);
		virtual ~Trident();
		static Trident* spawnAdjusted(float x, float y, int dir, TextureManager &textureManager);
};

#endif
