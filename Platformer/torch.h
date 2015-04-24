#ifndef TORCH_H
#define TORCH_H
// vim: noet

#include "weapon.h"

class Torch : public Weapon
{
	public:
		Torch(Room &room, float x, float y, int dir);
		virtual ~Torch();

		virtual void onDeath();
		static Torch* spawnAdjusted(Room &room, float x, float y, int dir);
};

#endif
