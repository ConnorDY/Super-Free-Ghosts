#include "projectile.h"

Projectile::Projectile(float x, float y, int dir, TextureManager &textureManager)
	: Object(
			Object::Type::Projectile,
			x, y, 30, 7,
			0.4f * dir, 0,
			false, false
	  )
{
	rectangle.setSize(sf::Vector2f(width, height));
	rectangle.setScale(sf::Vector2f((float)dir, 1.0f));

	// Sprite
	sprite.setTexture(textureManager.getRef("weapons"));
	sprite.setScale(sf::Vector2f((float)dir, 1.0f));

	weapons.push_back(sf::IntRect(26, 34, 30, 7));

	sprite.setTextureRect(weapons[0]);
}

Projectile::~Projectile()
{
}


/* Actions */
void Projectile::draw(sf::RenderWindow &window)
{
	if (DEBUG_MODE)
	{
		rectangle.setPosition(x, y);
		window.draw(rectangle);
	}

	sprite.setPosition(x, y);
	window.draw(sprite);
}

// TODO: Get rid of these by fixing the projectile's bounding box (i.e. make x & y the upper left of the bounding box as usual)
bool Projectile::placeFree_adj(float x, float y, std::vector<Object*> const objects) const {
	return placeFree(x + (dx < 0 ? -width : 0), y, objects);
}

Object* Projectile::placeEmpty_adj(float x, float y, std::vector<Object*> const objects) const {
	return placeEmpty(x + (dx < 0 ? -width : 0), y, objects);
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
	if (!placeFree_adj(x + sign_x, y + sign_y, objects)) setDelete();
	else if (x < viewx - 30.0f || x > viewx + WINDOW_WIDTH + 30.0f) setDelete(1);

	// Destroy projectile if it hits an enemy and destroy the enemy
	Object* col = placeEmpty_adj(x, y, objects);
	if (col != NULL && col->getType() == Object::Type::Zombie)
	{
		col->setDelete();
		this->setDelete();
	}
}
