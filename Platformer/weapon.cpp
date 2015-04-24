#include "weapon.h"
#include "globals.h"
#include "zombie.h"
#include "chest.h"
#include "room.h"

Weapon::Weapon(Room &room, float x, float y, float width, float height, float dx, float dy, float gravity, float maxFallSpeed, int damage)
	: Object(room, x, y, width, height, dx, dy, false, gravity, maxFallSpeed),
	  rectangle(sf::Vector2f(width, height)), dmg(damage), animationFrame(0),
	  animationSpeed(6), destroyedOnHit(true)
{
	rectangle.setFillColor(sf::Color(255, 0, 0, 128));
	sprite.setTexture(textureManager.getRef("weapons"));
	sprite.setTextureRect(sf::IntRect(0, 0, 0, 0));
}

Weapon::~Weapon() {}

bool Weapon::isOutsideView(float leeway) const
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

void Weapon::move(sf::Time deltaTime)
{
	float mstime = deltaTime.asMicroseconds() / 1000.0f;

	dy = std::min<float>(maxFallSpeed, dy + gravity * mstime);

	y += dy * mstime;
	x += dx * mstime;
}

void Weapon::update(sf::Time deltaTime)
{
	move(deltaTime);
	if (del) return; // Don't do anything if our move killed us

	for (Object* col : allCollisions(x, y))
	{
		// Destroy weapon if it hits an enemy and destroy the enemy
		if (dynamic_cast<Zombie*>(col) != nullptr)
		{
			if (!((Zombie*)col)->getInCasket()) ((Zombie*)col)->damage(dmg);
			if (destroyedOnHit)
				kill();
		}
		// Destroy weapon if it hits a chest and damage the chest
		else if (dynamic_cast<Chest*>(col) != nullptr)
		{
			((Chest*)col)->damage(dmg);
			if (destroyedOnHit)
				kill();
		}
	}

	// Destroy weapon if it hits a solid object or leaves the room
	if (!placeFree(x, y) || isOutsideView()) kill();

	animationFrame = fmod(animationFrame + deltaTime.asSeconds() * animationSpeed, animationFrames.size());
}

void Weapon::draw(sf::RenderWindow &window)
{
	if (!animationFrames.empty())
	{
		sprite.setPosition(x, y);
		sprite.setTextureRect(animationFrames[floor(animationFrame)]);
		window.draw(sprite);
	}

	if (DEBUG_MODE)
	{
		auto rect = getRect();
		rectangle.setPosition(rect.left, rect.top);
		rectangle.setSize(sf::Vector2f(rect.width, rect.height));
		window.draw(rectangle);
	}
}

void Weapon::onDeath()
{
	if (!isOutsideView()) room.soundManager.playSound("hit");
}
