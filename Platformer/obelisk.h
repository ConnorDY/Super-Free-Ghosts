#ifndef OBELISK_H
#define OBELISK_H

#include <SFML/Graphics.hpp>
#include <math.h>

#include "object.h"

class Obelisk : public Object
{
private:
	sf::Sprite spr;
public:
	explicit Obelisk(TextureManager &textureManager, float x = -128, float y = -128, float height = 72);

	// Actions
	virtual void draw(sf::RenderWindow &window);
};

#endif
