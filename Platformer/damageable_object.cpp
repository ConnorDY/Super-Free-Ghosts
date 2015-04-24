#include "damageable_object.h"

DamageableObject::DamageableObject(Room &room, float x, float y, float width, float height, float dx, float dy, bool solid, float gravity, float maxFallSpeed)
	: Object(
		room, x, y, width, height,
		dx, dy, solid,
		gravity, maxFallSpeed
	  ), health(10), team(Team::Neutral)
{
}

DamageableObject::~DamageableObject()
{

}

int DamageableObject::getHealth() const
{
	return health;
}

void DamageableObject::setHealth(int h)
{
	health = h;
}

void DamageableObject::damage(int dmg)
{
	health -= dmg;
	if (health <= 0) kill();
}
