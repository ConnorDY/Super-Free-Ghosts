#ifndef SWORD_H
#define SWORD_H
class Player;
#include "melee_weapon.h"

class Sword : public MeleeWeapon
{
	public:
		Sword(Room &room, Player const &player, int dir);
		virtual ~Sword();
};

#endif
