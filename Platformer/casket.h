#ifndef CASKET_H
#define CASKET_H

#include <SFML/Audio.hpp>
#include <stdlib.h>
#include <math.h>
#include <string>
#include <vector>
#include <map>

#include "object.h"

class Casket : public Object
{
	private:
		sf::RectangleShape rectangle;
		std::vector<sf::IntRect> const *animation;
		float frame, spawnX, spawnY, angle;
		bool opening, spawned, under, visible, finished;
		sf::Clock casketTimer, openTimer, flashTimer;

		// Actions
		void setAnimation(std::vector<sf::IntRect> const *anim);
		void updateAnimation(sf::Time deltaTime);
	public:
		Casket(Room &room, float x, float y);

		virtual bool canCollideWith(const Object *obj) const override;

		// Actions
		virtual void draw(sf::RenderWindow &window) override;
		virtual void update(sf::Time deltaTime) override;
};

#endif
