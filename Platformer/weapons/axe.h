#ifndef AXE_H
#define AXE_H
class Player;
#include "melee_weapon.h"

class Axe : public MeleeWeapon
{
	public:
		Axe(Room &room, Player const &player, int dir);
		virtual ~Axe();
};

#endif
