#include "blood_particle.h"

BloodParticle::BloodParticle(Room &room, float x, float y, int dir)
	: Particle(
		room, x, y, 1, 1,
		dir * 0.2f, 0,
		0.0014f / 2.0f, 0.2f
	)
{
	rect.setSize(sf::Vector2f(width, height));
	rect.setFillColor(sf::Color(255, 0, 0));
}

BloodParticle::~BloodParticle()
{
}

void BloodParticle::draw(sf::RenderWindow &window)
{
	rect.setPosition(x, y);
	window.draw(rect);
}

void BloodParticle::update(sf::Time deltaTime)
{
	Particle::update(deltaTime);

	if (!placeFree(x, y + 1.0f)) deactivate();
}

void BloodParticle::onDeath()
{
	std::cout << "lel" << std::endl;
}
