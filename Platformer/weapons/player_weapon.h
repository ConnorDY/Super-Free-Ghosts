#ifndef PLAYER_WEAPON_H
#define PLAYER_WEAPON_H
class Weapon;
class Player;
class Room;

namespace PlayerWeapon
{
	enum Enum {
		SPEAR, BOMB, TORCH, TRIDENT, HAMMER, SWORD
	};

	Weapon *spawnWeapon(Room &room, Player *player, Enum weapon, bool super, float x, float y, int dir);
	bool canThrow(Room const &room, Enum weapon);
	bool isMeleeWeapon(Enum weapon);
}

#endif
