#ifndef ZOMBIE_H
#define ZOMBIE_H

#include <SFML/Audio.hpp>
#include <stdlib.h>
#include <math.h>
#include <string>
#include <vector>
#include <map>

#include "damageable_object.h"

class Zombie : public DamageableObject
{
	private:
		sf::RectangleShape rectangle;
		std::vector<sf::IntRect> const *animation;
		float moveSpeed, frame;
		bool turning;
		sf::Clock turnTimer;

		// Actions
		void setAnimation(std::vector<sf::IntRect> const *anim);
		void updateAnimation(sf::Time deltaTime);
	public:
		Zombie(Room &room, float x, float y);

		// Accesors
		int getDir() const;
		virtual bool canCollideWith(const Object* obj) const override;

		// Actions
		virtual void draw(sf::RenderWindow &window) override;
		virtual void update(sf::Time deltaTime) override;
		virtual void onDeath() override;
};

#endif
