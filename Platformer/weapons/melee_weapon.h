#ifndef MELEE_WEAPON_H
#define MELEE_WEAPON_H

#include <SFML/Graphics/Rect.hpp>

class Player;
#include "weapon.h"

class MeleeWeapon : public Weapon
{
	private:
		Player const &player;
		sf::IntRect boundingBox;
		int dir;
	protected:
		virtual bool isCollidable() override;
	public:
		MeleeWeapon(Room &room, Player const &player, sf::IntRect boundingBox, int dir, int damage);
		virtual ~MeleeWeapon();

		virtual sf::FloatRect getRect() const override;
		virtual void move(sf::Time deltaTime) override;
		virtual void update(sf::Time deltaTime) override;
};

#endif
