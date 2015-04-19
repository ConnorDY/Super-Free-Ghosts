#ifndef SUPER_SPEAR_H
#define SUPER_SPEAR_H

#include "spear.h"

class SuperSpear : public Spear
{
	public:
		SuperSpear(float x, float y, int dir, TextureManager &textureManager);
		virtual ~SuperSpear();
		static SuperSpear* spawnAdjusted(float x, float y, int dir, TextureManager &textureManager);
};

#endif
