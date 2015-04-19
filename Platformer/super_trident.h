#ifndef SUPER_TRIDENT_H
#define SUPER_TRIDENT_H

#include "trident.h"

class SuperTrident : public Trident
{
	public:
		SuperTrident(float x, float y, int dir, TextureManager &textureManager);
		virtual ~SuperTrident();
		static SuperTrident* spawnAdjusted(float x, float y, int dir, TextureManager &textureManager);
};

#endif
