#ifndef PARTICLE_H
#define PARTICLE_H

#include "object.h"

class Particle : public Object
{
	private:
		sf::Clock timer;
		double lifetime;
		bool active;
	public:
		explicit Particle(
			Room &room,
			float x = 0, float y = 0,
			float width = 0, float height = 0,
			float dx = 0, float dy = 0,
			float gravity = 0,
			float maxFallSpeed = 0,
			double lifetime = 0
		);
		~Particle();

		bool isActive() const;
		void deactivate();

		virtual void update(sf::Time deltaTime);
};

#endif
