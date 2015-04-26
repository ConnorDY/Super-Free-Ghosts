#include "weapon.h"
#include "damageable_object.h"
#include "globals.h"
#include "room.h"

Weapon::Weapon(Room &room, float x, float y, float width, float height, float dx, float dy, float gravity, float maxFallSpeed, int damage)
	: Object(room, x, y, width, height, dx, dy, false, gravity, maxFallSpeed),
	  rectangle(sf::Vector2f(width, height)), dmg(damage), animationCycle(0),
	  animationFrame(0), animationSpeed(6), destroyedOnHit(true),
	  destroyedOnSolidCollision(true), destroyedOnExitView(true)
{
	rectangle.setFillColor(sf::Color(255, 0, 0, 128));
}

Weapon::~Weapon() {}

void Weapon::doNotDamage(Object *what)
{
	alreadyAttacked.insert(what);
}

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
		auto damageable = dynamic_cast<DamageableObject*>(col);

		// Destroy weapon if it hits an enemy and destroy the enemy
		if (damageable != nullptr)
		{
			auto haveNotAttacked = alreadyAttacked.insert(damageable).second;
			if (haveNotAttacked)
			{
				damageable->damage(this, dmg);
				if (destroyedOnHit)
					kill();
			}
		}
	}

	if (destroyedOnSolidCollision && !placeFree(x, y)) kill();
	if (destroyedOnExitView && isOutsideView()) kill();

	animationFrame += deltaTime.asSeconds() * animationSpeed;
	auto frames = animationFrames.size();
	animationCycle += static_cast<int>(floor(animationFrame / frames));
	animationFrame = fmod(animationFrame, frames);
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
