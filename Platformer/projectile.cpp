#include "projectile.h"

Projectile::Projectile(float xx, float yy, int dir, TextureManager &textureManager)
{
	setX(xx);
	setY(yy);
	setDX((float)dir / 1.25f);
	
	setWidth(60.0f);
	setHeight(12.0f);
	setSolid(false);
	setType(Object::Type::Projectile);

	rectangle.setSize(sf::Vector2f(getWidth(), getHeight()));
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
	//rectangle.setPosition(roundf(getX()), roundf(getY()));
	//window.draw(rectangle);

	sprite.setPosition(roundf(getX()), roundf(getY()));
	window.draw(sprite);
}

void Projectile::update(sf::Time deltaTime, float viewx, std::vector<Object*> objects)
{
	Object::update(deltaTime, objects);

	// Determine the direction the projectile is moving in
	float sign_x = 0, sign_y = 0;

	if (getDX() > 0.0f) sign_x = 1.0f;
	else if (getDX() < 0.0f) sign_x = -1.0f;

	if (getDY() > 0.0f) sign_y = 1.0f;
	else if (getDY() < 0.0f) sign_y = 1.0f;

	// Destroy projectile if it hits a solid object or leaves the room
	if (!placeFree(getX() + sign_x, getY() + sign_y, objects)) setDelete(true);
	else if (getX() < viewx - 100.0f || getX() > viewx + WINDOW_WIDTH + 100.0f) setDelete(true);
}
