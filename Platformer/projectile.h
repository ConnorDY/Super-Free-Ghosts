#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <SFML/Graphics.hpp>
#include "texture_manager.h"
#include "object.h"

class Projectile : public Object
{
	private:
		sf::RectangleShape rectangle;
		std::vector<sf::IntRect> weapons;
	public:
		Projectile(float x, float y, int dir, TextureManager &textureManager);
		virtual ~Projectile();

		// Actions
		virtual void draw(sf::RenderWindow &window);
		// TODO: Doesn't override (can we put viewx in object? does this need viewx?)
		virtual void update(sf::Time deltaTime, float viewx, std::vector<Object*> objects);
};

#endif
