#ifndef WEAPON_H
#define WEAPON_H

#include "object.h"

class Weapon : public Object {
	protected:
		sf::RectangleShape rectangle;
		bool isOutsideView(Room const &room, float leeway = 30) const;
	public:
		Weapon(float x, float y, float width, float height, float dx, float dy, float gravity, float maxFallSpeed, TextureManager &textureManager);
		virtual ~Weapon();

		virtual void move(sf::Time deltaTime, Room &room, settings_t const &settings);
		void draw(sf::RenderWindow &window);
		virtual void update(sf::Time deltaTime, Room &room, settings_t const &settings);
		virtual void onDeath(Room &room, settings_t const &settings);
};

#endif
