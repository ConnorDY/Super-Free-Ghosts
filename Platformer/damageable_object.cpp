#include "damageable_object.h"

DamageableObject::DamageableObject(float x, float y, float width, float height, float dx, float dy, bool solid, float gravity, float maxFallSpeed)
	: Object(
	x, y, width, height,
	dx, dy, solid,
	gravity, maxFallSpeed)
{
	health = 1;
	team = Team::Neutral;
}

DamageableObject::~DamageableObject()
{

}

void DamageableObject::setHealth(int h)
{
	health = h;
}

void DamageableObject::damage(const Room &room, const settings_t &settings)
{
	if (health > 1) health--;
	else kill(room, settings);
}

void DamageableObject::update(sf::Time, Room const&, const settings_t&)
{

}
