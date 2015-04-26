#ifndef WEAPONDROP_H
#define WEAPONDROP_H

#include "object.h"
#include "weapons/player_weapon.h"

class WeaponDrop : public Object
{
	private:
		sf::RectangleShape rect;
		PlayerWeapon::Enum type;
	public:
		WeaponDrop(Room &room, TextureManager &textureManager, float x, float y);
		~WeaponDrop();

		// Actions
		virtual void draw(sf::RenderWindow &window) override;
		virtual void update(sf::Time deltaTime) override;
};

#endif
