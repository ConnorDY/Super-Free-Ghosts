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

	float adj = 0;
	if (dx < 0.0f) adj = width;

	sprite.setPosition(x + adj, y);
	window.draw(sprite);
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

void Projectile::update(sf::Time deltaTime, Room const &room, const settings_t &settings)
{
	Object::update(deltaTime, room, settings);

	// Determine the direction the projectile is moving in
	float sign_x = 0;

	// TODO: clean up
	if (dx > 0.0f) sign_x = 1.0f;
	else if (dx < 0.0f) sign_x = -1.0f;

	// Destroy projectile if it hits a solid object or leaves the room
	if (!placeFree(x + sign_x, y, room) || isOutsideView(room)) kill(room, settings);

	// Destroy projectile if it hits an enemy and destroy the enemy
	Object* col = nonsolidCollision(x, y, room);
	if (dynamic_cast<Zombie*>(col) != NULL)
	{
		if (!((Zombie*)col)->getInCasket()) col->kill(room, settings);
		this->kill(room, settings);
	}
}

void Projectile::onDeath(Room const &room, const settings_t &settings)
{
	if (!isOutsideView(room) && settings.sound_on) room.getSoundManager().playSound("hit");
}
