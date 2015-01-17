#include "projectile.h"

Projectile::Projectile(float x, float y, int dir, TextureManager &textureManager)
	: Object(
			Object::Type::Projectile,
			x, y, 60, 12,
			0.8f * dir, 0,
			false, false
	  )
{
	rectangle.setSize(sf::Vector2f(width, height));
	rectangle.setScale(sf::Vector2f((float)dir, 1.0f));

	// Sprite
	sprite.setTexture(textureManager.getRef("weapons"));
	sprite.setScale(sf::Vector2f(2.0f * (float)dir, 2.0f));

	weapons.push_back(sf::IntRect(26, 34, 30, 7));

	sprite.setTextureRect(weapons[0]);
}

Projectile::~Projectile()
{
}


/* Actions */
void Projectile::draw(sf::RenderWindow &window)
{
	//rectangle.setPosition(roundf(x), roundf(y));
	//window.draw(rectangle);

	sprite.setPosition(roundf(x), roundf(y));
	window.draw(sprite);
}

void Projectile::update(sf::Time deltaTime, float viewx, std::vector<Object*> objects)
{
	Object::update(deltaTime, objects);

	// Determine the direction the projectile is moving in
	float sign_x = 0, sign_y = 0;

	if (dx > 0.0f) sign_x = 1.0f;
	else if (dx < 0.0f) sign_x = -1.0f;

	if (dy > 0.0f) sign_y = 1.0f;
	else if (dy < 0.0f) sign_y = 1.0f;

	// Destroy projectile if it hits a solid object or leaves the room
	if (!placeFree(x + sign_x, y + sign_y, objects)) setDelete();
	else if (x < viewx - 100.0f || x > viewx + WINDOW_WIDTH + 100.0f) setDelete(1);

	// Destroy projectile if it hits an enemy and destroy the enemy
	Object* col = placeEmpty(x, y, objects);
	if (col != NULL && col->getType() == Object::Type::Zombie)
	{
		col->setDelete();
		this->setDelete();
	}
}
