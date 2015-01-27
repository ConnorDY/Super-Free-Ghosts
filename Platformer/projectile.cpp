#include "projectile.h"
#include "zombie.h"

Projectile::Projectile(float x, float y, int dir, TextureManager &textureManager)
	: Object(
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
bool Projectile::placeFree_adj(float x, float y, Room const &room) const {
	return placeFree(x + (dx < 0 ? -width : 0), y, room);
}

Object* Projectile::nonsolidCollision_adj(float x, float y, Room const &room) const {
	return nonsolidCollision(x + (dx < 0 ? -width : 0), y, room);
}

bool Projectile::isOutsideView(Room const &room) const
{
	auto view = room.getView();
	auto view_center = view.getCenter();
	auto view_size = view.getSize();

	float view_left = view_center.x - view_size.x/2;
	float view_right = view_center.x + view_size.x/2;
	return x < view_left - 30.0f || x > view_right + 30.0f;
}

void Projectile::update(sf::Time deltaTime, Room const &room)
{
	Object::update(deltaTime, room);

	// Determine the direction the projectile is moving in
	float sign_x = 0, sign_y = 0;

	// TODO: clean up
	if (dx > 0.0f) sign_x = 1.0f;
	else if (dx < 0.0f) sign_x = -1.0f;

	if (dy > 0.0f) sign_y = 1.0f;
	else if (dy < 0.0f) sign_y = 1.0f;

	// Destroy projectile if it hits a solid object or leaves the room
	if (!placeFree_adj(x + sign_x, y + sign_y, room) || isOutsideView(room)) kill(room);

	// Destroy projectile if it hits an enemy and destroy the enemy
	Object* col = nonsolidCollision_adj(x, y, room);
	if (dynamic_cast<Zombie*>(col) != NULL)
	{
		col->kill(room);
		this->kill(room);
	}
}

void Projectile::onDeath(Room const &room)
{
	if (!isOutsideView(room))
		room.getSoundManager().playSound("hit");
}
