#include "projectile.h"

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

	setDepth(-4);
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
	// Update X
	x += dx * (float)deltaTime.asMilliseconds();

	// Destroy projectile if it hits an enemy and destroy the enemy
	Object* col = nonsolidCollision(x, y, room);

	if (dynamic_cast<Zombie*>(col) != nullptr)
	{
		if (!((Zombie*)col)->getInCasket()) ((Zombie*)col)->damage(room, settings);
		kill(room, settings);
	}
	
	// Destory projectile if it hits a chest and damage the chest
	col = solidCollision(x, y, room);
	if (dynamic_cast<Chest*>(col) != nullptr)
	{
		((Chest*)col)->damage(room, settings);
		kill(room, settings);
	}

	// Destroy projectile if it hits a solid object or leaves the room
	if (!placeFree(x, y, room) || isOutsideView(room)) kill(room, settings);
}

void Projectile::onDeath(Room const &room, const settings_t &settings)
{
	if (!isOutsideView(room) && settings.sound_on) room.getSoundManager().playSound("hit");
}
