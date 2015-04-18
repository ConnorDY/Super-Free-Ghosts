#ifndef SPEAR_H
#define SPEAR_H

#include <SFML/Graphics.hpp>
#include "weapon.h"

class Spear : public Weapon
{
	public:
		Spear(float x, float y, int dir, TextureManager &textureManager);
		virtual ~Spear();
		static Spear* spawnAdjusted(float x, float y, int dir, TextureManager &textureManager);
};

#endif
