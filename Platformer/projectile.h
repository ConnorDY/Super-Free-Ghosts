#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <SFML/Graphics.hpp>
#include "texture_manager.h"
#include "object.h"

class Projectile : public Object
{
	private:
		sf::RectangleShape rectangle;
		sf::Sprite sprite;
		std::vector<sf::IntRect> weapons;
	public:
		Projectile(float xx, float yy, int dir, TextureManager &textureManager);

		// Actions
		void draw(sf::RenderWindow &window);
		void update(sf::Time deltaTime, float viewx, std::vector<Object*> objects);
};

#endif
