#include "projectile.h"

Projectile::Projectile(float x, float y, int dir, TextureManager &textureManager)
	: Weapon(x, y, 30, 7, 0.4f * dir, 0, 0, 0, 10, textureManager)
{
	sprite.setScale(sf::Vector2f((float)dir, 1.0f));
	sprite.setTextureRect(sf::IntRect(26, 34, 30, 7));

	setDepth(-4);
}

Projectile::~Projectile()
{
}
