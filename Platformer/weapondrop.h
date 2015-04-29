#ifndef WEAPONDROP_H
#define WEAPONDROP_H

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <vector>
#include "globals.h"
#include "object.h"
#include "weapons/player_weapon.h"

class WeaponDrop : public Object
{
	private:
		sf::Sprite innerGlow, outerGlow;
		DEBUG(sf::RectangleShape rect;)
		std::vector<sf::IntRect> animation;
		PlayerWeapon::Enum type;
		float animationFrame;

		void setWeaponAnimation();
		void updateAnimation(sf::Time deltaTime);
	public:
		WeaponDrop(Room &room, float x, float y, PlayerWeapon::Enum type);
		~WeaponDrop();

		// Actions
		virtual void draw(sf::RenderWindow &window) override;
		virtual void update(sf::Time deltaTime) override;
};

#endif
