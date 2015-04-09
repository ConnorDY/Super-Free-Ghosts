#ifndef BLOOD_PARTICLE_H
#define BLOOD_PARTICLE_H

#include "particle.h"

class BloodParticle : public Particle
{
	private:
		sf::RectangleShape rect;
	public:
		BloodParticle(float x, float y, int dir);
		~BloodParticle();

		virtual void draw(sf::RenderWindow &window);
		virtual void update(sf::Time deltaTime, Room &room, const settings_t &settings);
		virtual void onDeath(Room &room, const settings_t &settings);
};

#endif
