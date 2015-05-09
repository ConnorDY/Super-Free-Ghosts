#include <stdexcept>
#include "player_weapon.h"

#include "super_spear.h"
#include "spear.h"
#include "super_trident.h"
#include "trident.h"
#include "super_hammer.h"
#include "hammer.h"
#include "sword.h"
#include "war_hammer.h"
#include "axe.h"
#include "torch.h"

namespace PlayerWeapon
{
	Weapon* spawnWeapon(Room &room, Player *player, Enum weapon, bool super, float x, float y, int dir)
	{
		switch (weapon)
		{
			case SPEAR:
				if (super) return SuperSpear::spawnAdjusted(room, x, y, dir);
				else return Spear::spawnAdjusted(room, x, y, dir);
			case TORCH:
				return Torch::spawnAdjusted(room, x, y, dir);
			case HAMMER:
				if (super) return SuperHammer::spawnAdjusted(room, x, y, dir);
				else return Hammer::spawnAdjusted(room, x, y, dir);
			case TRIDENT:
				if (super) return SuperTrident::spawnAdjusted(room, x, y, dir);
				else return Trident::spawnAdjusted(room, x, y, dir);
			case SWORD:
				return new Sword(room, *player, dir);
			case WAR_HAMMER:
				return new War_Hammer(room, *player, dir);
			case AXE:
				return new Axe(room, *player, dir);
			default:
				throw std::domain_error("Tried to spawn a nonexistent weapon");
		}
	}

	bool canThrow(Room const &room, Enum weapon)
	{
		if (weapon == TRIDENT) return Trident::canThrow(room);
		return true;
	}

	bool isMeleeWeapon(Enum weapon)
	{
		switch (weapon)
		{
			case SPEAR:
			case TORCH:
			case HAMMER:
			case TRIDENT:
				return false;

			case SWORD:
			case WAR_HAMMER:
			case AXE:
				return true;

			default:
				throw std::domain_error("Unknown weapon when checking for melee/ranged");
		}
	}
}
