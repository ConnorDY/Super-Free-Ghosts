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

		bool isOutsideView(Room const &room) const;
		bool placeFree_adj(float x, float y, std::vector<Object*> const objects) const;
		Object* nonsolidCollision_adj(float x, float y, std::vector<Object*> const objects) const;
	public:
		Projectile(float x, float y, int dir, TextureManager &textureManager);
		virtual ~Projectile();

		// Actions
		virtual void draw(sf::RenderWindow &window);
		virtual void update(sf::Time deltaTime, Room const &room);
		virtual void onDeath(Room const &room);
};

#endif
