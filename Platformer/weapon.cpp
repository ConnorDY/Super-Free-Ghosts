#include "weapon.h"
#include "zombie.h"
#include "chest.h"
#include "room.h"

Weapon::Weapon(float x, float y, float width, float height, float dx, float dy, float gravity, float maxFallSpeed, TextureManager &textureManager)
	: Object(x, y, width, height, dx, dy, false, gravity, maxFallSpeed),
	  rectangle(sf::Vector2f(width, height))
{
	sprite.setTexture(textureManager.getRef("weapons"));
	sprite.setTextureRect(sf::IntRect(0, 0, 0, 0));
}

Weapon::~Weapon() {}

bool Weapon::isOutsideView(Room const &room, float leeway) const
{
	auto view = room.getView();
	auto view_center = view.getCenter();
	auto view_size = view.getSize();

	float view_left = view_center.x - view_size.x/2;
	float view_right = view_center.x + view_size.x/2;
	float view_top = view_center.y - view_size.y/2;
	float view_bottom = view_center.y + view_size.y/2;

	return x < view_left - leeway || x > view_right  + leeway
		|| y < view_top  - leeway || y > view_bottom + leeway;
}

void Weapon::move(sf::Time deltaTime, Room&, settings_t const&)
{
	float mstime = deltaTime.asMicroseconds() / 1000.0f;

	dy = std::min<float>(maxFallSpeed, dy + gravity * mstime);

	y += dy * mstime;
	x += dx * mstime;
}

void Weapon::update(sf::Time deltaTime, Room &room, settings_t const &settings)
{
	move(deltaTime, room, settings);
	if (del) return; // Don't do anything if our move killed us

	for (Object* col : allCollisions(x, y, room))
	{
		// Destroy weapon if it hits an enemy and destroy the enemy
		if (dynamic_cast<Zombie*>(col) != nullptr)
		{
			if (!((Zombie*)col)->getInCasket()) ((Zombie*)col)->damage(room, settings);
			kill(room, settings);
		}
		// Destroy weapon if it hits a chest and damage the chest
		else if (dynamic_cast<Chest*>(col) != nullptr)
		{
			((Chest*)col)->damage(room, settings);
			kill(room, settings);
		}
	}

	// Destroy weapon if it hits a solid object or leaves the room
	if (!placeFree(x, y, room) || isOutsideView(room)) kill(room, settings);
}

void Weapon::draw(sf::RenderWindow &window)
{
	if (DEBUG_MODE)
	{
		auto rect = getRect();
		rectangle.setPosition(rect.left, rect.top);
		rectangle.setSize(sf::Vector2f(rect.width, rect.height));
		window.draw(rectangle);
	}

	float adj = 0;
	if (dx < 0.0f) adj = width;

	sprite.setPosition(x + adj, y);
	window.draw(sprite);
}

void Weapon::onDeath(Room &room, const settings_t &settings)
{
	if (!isOutsideView(room) && settings.sound_on) room.getSoundManager().playSound("hit");
}
