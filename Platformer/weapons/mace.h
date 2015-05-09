#ifndef MACE_H
#define MACE_H
class Player;
#include "melee_weapon.h"

class Mace : public MeleeWeapon
{
	public:
		Mace(Room &room, Player const &player, int dir);
		virtual ~Mace();
};

#endif
