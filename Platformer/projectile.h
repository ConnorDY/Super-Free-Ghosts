#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <SFML/Graphics.hpp>
#include "weapon.h"

class Projectile : public Weapon
{
	public:
		Projectile(float x, float y, int dir, TextureManager &textureManager);
		virtual ~Projectile();
};

#endif
