#include "blood_particle.h"

BloodParticle::BloodParticle(float x, float y, int dir)
	: Particle(
		x, y, 1, 1,
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

void BloodParticle::update(sf::Time deltaTime, Room &room, const settings_t &settings)
{
	Particle::update(deltaTime, room, settings);

	if (!placeFree(x, y + 1.0f, room)) deactivate();
}

void BloodParticle::onDeath(Room &, const settings_t &)
{
	std::cout << "lel" << std::endl;
}
