#ifndef WAR_HAMMER_H
#define WAR_HAMMER_H
class Player;
#include "melee_weapon.h"

class War_Hammer : public MeleeWeapon
{
	public:
		War_Hammer(Room &room, Player const &player, int dir);
		virtual ~War_Hammer();
};

#endif
