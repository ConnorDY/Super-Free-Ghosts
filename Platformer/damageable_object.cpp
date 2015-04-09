#include "damageable_object.h"

DamageableObject::DamageableObject(float x, float y, float width, float height, float dx, float dy, bool solid, float gravity, float maxFallSpeed)
	: Object(
	x, y, width, height,
	dx, dy, solid,
	gravity, maxFallSpeed), health(10), team(Team::Neutral)
{
}

DamageableObject::~DamageableObject()
{

}

void DamageableObject::setHealth(int h)
{
	health = h;
}

void DamageableObject::damage(int dmg, Room &room, const settings_t &settings)
{
	health -= dmg;
	if (health <= 0) kill(room, settings);
}
