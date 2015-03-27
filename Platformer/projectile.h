#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <SFML/Graphics.hpp>
#include "texture_manager.h"
#include "object.h"
#include "zombie.h"
#include "settings.h"

class Projectile : public Object
{
	private:
		sf::RectangleShape rectangle;
		std::vector<sf::IntRect> weapons;

		bool isOutsideView(Room const &room) const;
	public:
		Projectile(float x, float y, int dir, TextureManager &textureManager);
		virtual ~Projectile();

		// Actions
		virtual void draw(sf::RenderWindow &window);
		virtual void update(sf::Time deltaTime, Room const &room, const settings_t &settings);
		virtual void onDeath(Room const &room, const settings_t &settings);
};

#endif
