#ifndef CHEST_H
#define CHEST_H

#include <SFML/Graphics.hpp>
#include <math.h>
#include <iostream>

#include "object.h"

class Chest : public Object
{
private:
	sf::RectangleShape rect;
	int health;
public:
	explicit Chest(TextureManager &textureManager, float x = -128, float y = -128);

	// Actions
	void damage(const Room &room, const settings_t &settings);
	virtual void update(sf::Time deltaTime, Room const &room, const settings_t &settings);
	virtual void draw(sf::RenderWindow &window);
};

#endif
