#ifndef WEAPONDROP_H
#define WEAPONDROP_H

#include "object.h"
#include "player.h"

class WeaponDrop : public Object
{
	private:
		sf::RectangleShape rect;
		PlayerWeapon::Enum type;
	public:
		WeaponDrop(TextureManager &textureManager, float x, float y);
		~WeaponDrop();

		// Actions
		virtual void draw(sf::RenderWindow &window);
		virtual void update(sf::Time deltaTime, Room &room, const settings_t &settings);
};

#endif