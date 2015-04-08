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
		float slideUpFraction;  // How far it is in its slide between underground and visible
		bool slidingUp;
	public:
		explicit Chest(TextureManager &textureManager, float x = -128, float y = -128);

		virtual sf::FloatRect getRect() const;

		// Actions
		virtual void update(sf::Time deltaTime, Room &room, const settings_t &settings);
		virtual void draw(sf::RenderWindow &window);
		virtual void onDoubleJumpedOver(Room &room);
};

#endif
