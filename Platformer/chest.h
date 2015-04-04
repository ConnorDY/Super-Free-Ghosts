#ifndef CHEST_H
#define CHEST_H

#include <SFML/Graphics.hpp>
#include <math.h>
#include <iostream>

#include "damageable_object.h"

class Chest : public DamageableObject
{
	private:
		sf::RectangleShape rect;
	public:
		explicit Chest(TextureManager &textureManager, float x = -128, float y = -128);

		// Actions
		virtual void update(sf::Time deltaTime, Room const &room, const settings_t &settings);
		virtual void draw(sf::RenderWindow &window);
};

#endif
