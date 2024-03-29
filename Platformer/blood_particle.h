#ifndef BLOOD_PARTICLE_H
#define BLOOD_PARTICLE_H

#include "particle.h"

class BloodParticle : public Particle
{
	private:
		sf::RectangleShape rect;
	public:
		BloodParticle(Room &room, float x, float y, int dir);
		~BloodParticle();

		virtual void draw(sf::RenderWindow &window) override;
		virtual void update(sf::Time deltaTime) override;
		virtual void onDeath() override;
};

#endif
