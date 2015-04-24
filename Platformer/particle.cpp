#include "particle.h"

Particle::Particle(Room &room, float x, float y, float width, float height, float dx, float dy, float gravity, float maxFallSpeed, double l)
	: Object(
		room, x, y, width, height,
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

void Particle::update(sf::Time deltaTime)
{
	Object::update(deltaTime);

	if (!active && timer.getElapsedTime().asMilliseconds() >= lifetime)	kill();
}
