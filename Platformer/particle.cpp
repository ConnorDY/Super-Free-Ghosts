#include "particle.h"

Particle::Particle(float x, float y, float width, float height, float dx, float dy, float gravity, float maxFallSpeed, double l)
	: Object(
		x, y, width, height,
		dx, dy, false,
		gravity, maxFallSpeed
	),
	lifetime(l), active(true)
{
}

Particle::~Particle()
{

}

bool Particle::isActive() const
{
	return active;
}

void Particle::deactivate()
{
	active = false;
	timer.restart();
}

void Particle::update(sf::Time deltaTime, Room &room, const settings_t &settings)
{
	Object::update(deltaTime, room, settings);

	if (!active && timer.getElapsedTime().asMilliseconds() >= lifetime)	kill(room, settings);
}

void Particle::onDeath(Room &room, const settings_t &settings)
{
}