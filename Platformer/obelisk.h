#ifndef OBELISK_H
#define OBELISK_H

#include <math.h>

#include "object.h"

class Obelisk : public Object
{
	private:
		sf::Sprite spr;
		std::vector<sf::IntRect> flames;
		double frame = 0;
		int dir = 1;
	public:
		explicit Obelisk(Room &room, float x = -128, float y = -128, float height = 72);

		// Actions
		virtual void update(sf::Time deltaTime) override;
		virtual void draw(sf::RenderWindow &window) override;
};

#endif
